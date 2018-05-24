/**
 * Copyright 2017 Institut Mines-Télécom
 *
 * This file is part of s2-dev-2017-vehicle-simulator.
 *
 * s2-dev-2017-vehicle-simulator is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * s2-dev-2017-vehicle-simulator is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with s2-dev-2017-vehicle-simulator. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

package fr.enstb.osvdatasimulator.main;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import javax.swing.SwingWorker;

/**
 * @author guillaumelg
 *
 */
public class VehicleBehaviourManager extends SwingWorker<Void, Float> {

	private static final long CHECK_CELLS_INTERVAL = 6000;

	/**
	 * Battery capacity in Ah (not Wh).
	 */
	private static final float BATTERY_CAPACITY_AH = 156f;

	private static final long CHECK_CHARGING_SOC_INTERVAL = 500;

	/**
	 * 15 seconds.
	 */
	private static long CHECK_CHANGE_INTERVAL = 15000;

	MainWindow mw;
	private volatile boolean isRunning = false;
	private boolean stop = false;
	private volatile float speed = 0f;
	private volatile float previousSpeed = 0f;
	private volatile float targetSpeed = 0f;
	private volatile long checkTime = 0;
	private volatile float distance;
	private volatile float soc;
	private volatile float totalVehicleDistance;
	private volatile boolean vehicleStoped = false;
	private volatile boolean vehicleShouldStop = false;

	private int previousSoc = 0;

	private long checkCellsTime = 0;
	private long checkBatteryChargingTime = 0;

	private float cellVoltageAverage;
	List<Float> cellVoltages = new ArrayList<Float>();
	List<Float> temperatures = new ArrayList<Float>();

	private float cellVoltageMin;
	private float cellVoltageMax;
	private float batteryVoltage = 82.0f;
	private float batteryCurrent = 0.0f;
	private float previousBatteryCurrent = 0.0f;
	private float batteryPower = 0.0f;
	private boolean isCharging = false;

	private float previousTotalVehicleDistance = 0.0f;

	// private VehicleChargingManager vehicleChargingManager;

	public VehicleBehaviourManager(MainWindow mw) {
		this.mw = mw;

		// Assumed exactly 150km autonomy
		distance = ((float) Math.random()) * 100;
		calculateNextEVSOC();
		totalVehicleDistance = (int) (Math.random() * 10000.0) + distance;
		for(int i = 0 ; i < 4 ; i++) {
			temperatures.add(15f);
		}
	}

	public boolean isRunning() {
		return isRunning;
	}

	@Override
	protected Void doInBackground() throws Exception {
		isRunning = true;
		checkTime = (new Date()).getTime() - 12000;
		checkCellsTime = checkTime;
		updateIsChargingFile();
		updateIsStoppedFile();
		while (!stop) {
			calculateNextEVSpeed();
			calculateNextEVSOC();
			updateSpeedFile();
			updateSocFile();
			updateTotalDistanceFile();
			publish(speed);
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
			}
		}

		return null;
	}

	private void updateTotalDistanceFile() {
		if (previousTotalVehicleDistance != totalVehicleDistance) {
			Tools.writeToFile(mw.getPathToFiles() + App.SEPARATOR + DataFiles.TOTAL_DISTANCE,
					new Float(totalVehicleDistance).toString());
			previousTotalVehicleDistance = totalVehicleDistance;
		}
	}

	private void updateIsStoppedFile() {
		Tools.writeToFile(mw.getPathToFiles() + App.SEPARATOR + DataFiles.VEHICLE_STOPPED, vehicleStoped ? "1" : "0");
	}

	private void updateSocFile() {
		if ((int) soc != previousSoc) {
			Tools.writeToFile(mw.getPathToFiles() + App.SEPARATOR + DataFiles.SOC, new Integer((int) soc).toString());
			previousSoc = (int) soc;
		}
	}

	private void calculateNextEVSOC() {
		if (!isCharging) {
			float intervalDistance = speed / 18000;
			distance += intervalDistance;
			totalVehicleDistance += intervalDistance;
			soc = 100 - (distance / 1.5f);
			if (soc < 0) {
				soc = 0;
			}

			// CURRENT
			batteryCurrent = 220f * speed / 130f;
			
			for(int i = 0 ; i < temperatures.size() ; i++) {
				temperatures.set(i, 15f + ((float) Math.random()) * 10f);
			}

		} else {
			if (soc >= 100) {
				soc = 100;
				isCharging = false;
				updateIsChargingFile();
				mw.chargeVehicle.setText(MainWindow.CHARGE_VEHICLE);
			} else {
				long currentTimeMilli = (new Date()).getTime();

				if (currentTimeMilli - checkBatteryChargingTime > CHECK_CHARGING_SOC_INTERVAL) {

					soc = soc + batteryCurrent * ((float) (currentTimeMilli - checkBatteryChargingTime))
							/ (BATTERY_CAPACITY_AH * 1000f);

					batteryCurrent = 10f + (((float) Math.random()) - 0.5f);

					checkBatteryChargingTime = currentTimeMilli;
				}
			}

			distance = (100 - soc) * 1.5f;
		}

		calculateNextCellsStatus();

		// POWER
		batteryPower = batteryVoltage * batteryCurrent;
	}

	private void calculateNextCellsStatus() {
		if ((new Date()).getTime() - checkCellsTime > CHECK_CELLS_INTERVAL) {

			// CELLS
			checkCellsTime = (new Date()).getTime();
			cellVoltageAverage = 2.7f + ((float) soc) * 0.9f / 100.0f;
			cellVoltages.clear();
			for (int i = 0; i < 24; i++) {
				float deltaU = -0.1f + ((float) Math.random()) * 0.2f;
				cellVoltages.add(i, deltaU + cellVoltageAverage);
			}
			cellVoltageMin = 3.7f;
			cellVoltageMax = 2.7f;
			for (int i = 0; i < 24; i++) {
				if (cellVoltages.get(i) < cellVoltageMin) {
					cellVoltageMin = cellVoltages.get(i);
				}
				if (cellVoltages.get(i) > cellVoltageMax) {
					cellVoltageMax = cellVoltages.get(i);
				}
			}

			// VOLTAGE
			batteryVoltage = 0f;
			for (int i = 0; i < 24; i++) {
				batteryVoltage += cellVoltages.get(i);
			}

			// TEMPERATURES
			if(isCharging) {
				for(int i = 0 ; i < temperatures.size() - 1 ; i++) {
					temperatures.set(i, 15f + ((float) Math.random()) * 10f);
				}
				temperatures.set(temperatures.size() - 1, 45f + ((float) Math.random()) * 20f);
			} else {
				for(int i = 0 ; i < temperatures.size() ; i++) {
					temperatures.set(i, 15f + ((float) Math.random()) * 10f);
				}
			}
			
			if (isRunning) {
				updateEvBatteryCellsStatusFile();
				Tools.writeToFile(mw.getPathToFiles() + App.SEPARATOR + DataFiles.BATTERY_VOLTAGE,
						new Float(batteryVoltage).toString());
				updateTemperaturesSatusFiles();
			}
		}
	}

	private void updateTemperaturesSatusFiles() {
		for (int i = 0; i < temperatures.size() ; i++) {
			Tools.writeToFile(mw.getPathToFiles() + App.SEPARATOR + DataFiles.TEMPERATURE_N + i,
					temperatures.get(i).toString());
		}
	}

	private void updateEvBatteryCellsStatusFile() {
		Tools.writeToFile(mw.getPathToFiles() + App.SEPARATOR + DataFiles.CELL_VOLTAGE_AVERAGE,
				new Float(cellVoltageAverage).toString());
		Tools.writeToFile(mw.getPathToFiles() + App.SEPARATOR + DataFiles.CELL_VOLTAGE_MIN,
				new Float(cellVoltageMin).toString());
		Tools.writeToFile(mw.getPathToFiles() + App.SEPARATOR + DataFiles.CELL_VOLTAGE_MAX,
				new Float(cellVoltageMax).toString());
		for (int i = 0; i < 24; i++) {
			Tools.writeToFile(mw.getPathToFiles() + App.SEPARATOR + DataFiles.CELL_VOLTAGE_N + (i < 10 ? "0" + i : i),
					new Float(cellVoltages.get(i)).toString());
		}
	}

	private void updateSpeedFile() {
		if (speed != previousSpeed) {
			Tools.writeToFile(mw.getPathToFiles() + App.SEPARATOR + DataFiles.SPEED, new Float(speed).toString());

		}
		if (speed != previousSpeed || isCharging) {
			// CURRENT
			if(previousBatteryCurrent != batteryCurrent) {
			Tools.writeToFile(mw.getPathToFiles() + App.SEPARATOR + DataFiles.CURRENT,
					new Float(batteryCurrent).toString());
				previousBatteryCurrent = batteryCurrent;
			}

			// POWER
			Tools.writeToFile(mw.getPathToFiles() + App.SEPARATOR + DataFiles.POWER,
					new Float(batteryPower).toString());
		}

		previousSpeed = speed;
	}

	private void calculateNextEVSpeed() {
		if (speed == 0f) {
			vehicleStoped = true;
			if ((new Date()).getTime() - checkTime > CHECK_CHANGE_INTERVAL && !vehicleShouldStop) {
				if (Math.random() < 0.7d) {
					targetSpeed = 10f + ((float) Math.random()) * 120f;
					checkTime = new Date().getTime();
				}
			} else {
				if (targetSpeed > 0f) {
					speed = 0.1f;
				}
			}
		} else {
			vehicleStoped = false;
			if ((new Date()).getTime() - checkTime > CHECK_CHANGE_INTERVAL) {
				if (Math.random() < 0.35d || vehicleShouldStop) {
					targetSpeed = 0f;
				} else {
					targetSpeed = 10f + ((float) Math.random()) * 120f;
				}
				checkTime = new Date().getTime();
			}
			if (targetSpeed == 0f) {
				speed -= 2f + ((float) Math.random());
				if (speed <= 1) {
					speed = 0;
					vehicleStopped();
				}
			} else {
				if (Math.abs(targetSpeed - speed) > 2f) {
					if (speed < targetSpeed) {
						speed += 0.3f + ((float) Math.random());
					} else {
						speed -= 0.5f + ((float) Math.random());
					}
				} else {
					speed = targetSpeed - 1.0f + 2f * ((float) Math.random());
				}
			}
		}
	}

	private void vehicleStopped() {
		mw.startStopVehicle.setEnabled(true);
		updateIsStoppedFile();
		if (vehicleShouldStop) {
			mw.chargeVehicle.setEnabled(true);
		}
		// (vehicleChargingManager = new
		// VehicleChargingManager(this)).execute();
	}

	@Override
	protected void process(List<Float> speeds) {
		mw.updateSpeedLabel(speeds.get(speeds.size() - 1));
		mw.updateSocLabel(soc);
	}

	public void stop() {
		this.stop = true;
	}

	public void onStartStopVehhicle() {
		if (vehicleStoped) {
			vehicleStoped = false;
			vehicleShouldStop = false;
			targetSpeed = 10f + ((float) Math.random()) * 120f;
			updateIsStoppedFile();
			mw.startStopVehicle.setText(MainWindow.STOP_VEHICLE);
			mw.chargeVehicle.setEnabled(false);
			mw.chargeVehicle.setText(MainWindow.CHARGE_VEHICLE);
			if (isCharging) {
				isCharging = false;
				updateIsChargingFile();
			}
		} else {
			mw.startStopVehicle.setEnabled(false);
			vehicleShouldStop = true;
			mw.startStopVehicle.setText(MainWindow.START_VEHICLE);
		}
	}

	private void updateIsChargingFile() {
		Tools.writeToFile(mw.getPathToFiles() + App.SEPARATOR + DataFiles.IS_CHARGING, (isCharging ? "1" : "0"));
	}

	public int getSOC() {
		return (int) this.soc;
	}

	public void onChargeVehicle() {
		if (isCharging) {
			batteryCurrent = 0f;
			batteryPower = 0f;
			updateSpeedFile();
			isCharging = false;
			updateIsChargingFile();
			mw.chargeVehicle.setText(MainWindow.CHARGE_VEHICLE);
		} else {
			isCharging = true;
			updateIsChargingFile();
			updateSpeedFile();
			mw.chargeVehicle.setText(MainWindow.STOP_CHARGING_VEHICLE);
			checkBatteryChargingTime = (new Date()).getTime();
		}
	}
}
