/**
 * 
 */
package fr.enstb.osvdatasimulator.bms.main;

import java.io.File;
import java.io.IOException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardWatchEventKinds;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;
import java.util.ArrayList;
import java.util.List;

import fr.enstb.osv.BMSDataReader;
import fr.enstb.osvdatasimulator.bms.network.OSVBooleanCoAPResource;
import fr.enstb.osvdatasimulator.bms.network.OSVFloatCoAPResource;
import fr.enstb.osvdatasimulator.bms.network.OSVFloatListCoAPResource;

/**
 * @author guillaumelg
 *
 */
public class BMSDataReaderSimulated extends Thread implements BMSDataReader {
	private volatile boolean read = true;

	private static final String BATTERY_CURRENT = "battery_current";
	private static final String BATTERY_POWER = "battery_power";
	private static final String BATTERY_VOLTAGE = "battery_voltage";
	private static final String CELL_VOLTAGE_ = "cell_voltage_";
	private static final String CELL_VOLTAGE_AVERAGE = "cell_voltage_average";
	private static final String CELL_VOLTAGE_MAX = "cell_voltage_max";
	private static final String CELL_VOLTAGE_MIN = "cell_voltage_min";
	private static final String IS_CHARGING = "is_charging";
	private static final String SOC = "soc";
	private static final String TEMPERATURE_ = "temperature_";

	private float batteryCurrent = 0.0f;
	private float batteryVoltage = 0.0f;
	private float batteryPower = 0.0f;
	private float cellVoltageMin = 0.0f;
	private float cellVoltageMax = 0.0f;
	private float cellVoltageAverage = 0.0f;
	private boolean isCharging = false;
	private float soc = 0;

	private List<Float> cellsVoltage = new ArrayList<Float>();
	private List<Float> temperatures = new ArrayList<Float>();

	private OSVFloatCoAPResource batteryCurrentResource;
	private OSVFloatCoAPResource batteryVoltageResource;
	private OSVFloatCoAPResource batteryPowerResource;
	private OSVFloatCoAPResource cellVoltageMinResource;
	private OSVFloatCoAPResource cellVoltageMaxResource;
	private OSVFloatCoAPResource cellVoltageAverageResource;
	private OSVBooleanCoAPResource isChargingResource;
	private OSVFloatCoAPResource socResource;
	private OSVFloatListCoAPResource cellVoltagesListResource;
	private OSVFloatListCoAPResource temperaturesListResource;
	
	public static final String SEPARATOR = System.getProperty("file.separator");
	public static final String TMP = System.getProperty("java.io.tmpdir");
	public static final String OUR_FOLDER = "osvdatasimulator";

	/**
	 * Default constructor.
	 * 
	 * User must provide references to network resources, so that their values
	 * are updated as soon as possible, when a new value is received. "null" is
	 * an acceptable value. If null reference is provided, nothing will happen
	 * on value changed.
	 * 
	 * @param batteryCurrent
	 * @param batteryVoltage
	 * @param batteryPower
	 * @param cellVoltageMin
	 * @param cellVoltageMax
	 * @param cellVoltageAverage
	 * @param isCharging
	 * @param soc
	 * @param cellVoltagesList
	 */
	public BMSDataReaderSimulated(OSVFloatCoAPResource batteryCurrent, OSVFloatCoAPResource batteryVoltage,
			OSVFloatCoAPResource batteryPower, OSVFloatCoAPResource cellVoltageMin, OSVFloatCoAPResource cellVoltageMax,
			OSVFloatCoAPResource cellVoltageAverage, OSVBooleanCoAPResource isCharging, OSVFloatCoAPResource soc,
			OSVFloatListCoAPResource cellVoltagesList, OSVFloatListCoAPResource temperaturesList) {

		this.batteryCurrentResource = batteryCurrent;
		this.batteryVoltageResource = batteryVoltage;
		this.batteryPowerResource = batteryPower;
		this.cellVoltageMinResource = cellVoltageMin;
		this.cellVoltageMaxResource = cellVoltageMax;
		this.cellVoltageAverageResource = cellVoltageAverage;
		this.isChargingResource = isCharging;
		this.socResource = soc;
		this.cellVoltagesListResource = cellVoltagesList;
		this.temperaturesListResource = temperaturesList;

		for (int i = 0; i < 24; i++) {
			cellsVoltage.add(null);
		}
		
		for (int i = 0; i < 4; i++) {
			temperatures.add(0.0f);
		}
	}

	@Override
	public void run() {
		// Wait if directory does not exist.
		while (!(new File(getDefaultPathToFiles())).isDirectory()) {
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
			}
		}

		WatchService watchService;
		try {
			watchService = FileSystems.getDefault().newWatchService();

			Path directory = Paths.get(getDefaultPathToFiles());
			directory.register(watchService, StandardWatchEventKinds.ENTRY_MODIFY);
			WatchKey key = null;

			while (read) {
				key = watchService.take();
				if (key != null) {
					List<WatchEvent<?>> eventList = key.pollEvents();

					try {
						
						List<String> modifiedFiles = new ArrayList<String>();
						String fileName;
						
						for (int i = 0; i < eventList.size(); i++) {
							fileName = eventList.get(i).context().toString();
							if(!modifiedFiles.contains(fileName)) {
								modifiedFiles.add(fileName);
							}
						}
						for(String s : modifiedFiles) {
							processFileModifiedEvent(s);
						}
					} catch (NullPointerException e) {
					}
				}
				key.reset();
			}
		} catch (IOException | InterruptedException e) {
			e.printStackTrace();
		}

	}

	private synchronized void processFileModifiedEvent(String fileName) {
		String fileContent = Tools.readFile(getDefaultPathToFiles() + SEPARATOR + fileName);

		if (fileName.equals(BATTERY_CURRENT)) {
			batteryCurrent = Float.parseFloat(fileContent);
			if (batteryCurrentResource != null) {
				batteryCurrentResource.setValue(batteryCurrent);
			}
		} else if (fileName.equals(BATTERY_POWER)) {
			batteryPower = Float.parseFloat(fileContent);
			if (batteryPowerResource != null) {
				batteryPowerResource.setValue(batteryPower);
			}
		} else if (fileName.equals(BATTERY_VOLTAGE)) {
			batteryVoltage = Float.parseFloat(fileContent);
			if (batteryVoltageResource != null) {
				batteryVoltageResource.setValue(batteryVoltage);
			}
		} else if (fileName.equals(CELL_VOLTAGE_AVERAGE)) {
			cellVoltageAverage = Float.parseFloat(fileContent);
			if (cellVoltageAverageResource != null) {
				cellVoltageAverageResource.setValue(cellVoltageAverage);
			}
		} else if (fileName.equals(CELL_VOLTAGE_MAX)) {
			cellVoltageMax = Float.parseFloat(fileContent);
			if (cellVoltageMaxResource != null) {
				cellVoltageMaxResource.setValue(cellVoltageMax);
			}
		} else if (fileName.equals(CELL_VOLTAGE_MIN)) {
			cellVoltageMin = Float.parseFloat(fileContent);
			if (cellVoltageMinResource != null) {
				cellVoltageMinResource.setValue(cellVoltageMin);
			}
		} else if (fileName.equals(IS_CHARGING)) {
			isCharging = fileContent.equals("1");
			if (isChargingResource != null) {
				isChargingResource.setValue(isCharging);
			}
		} else if (fileName.equals(SOC)) {
			soc = Float.parseFloat(fileContent);
			if (socResource != null) {
				socResource.setValue(soc);
			}
		} else if (fileName.startsWith(CELL_VOLTAGE_)) {
			int cellIndex = Integer.parseInt(fileName.substring(13));
			float cellVoltage = Float.parseFloat(fileContent);
			cellsVoltage.set(cellIndex, cellVoltage);
			if (cellVoltagesListResource != null) {
				cellVoltagesListResource.setElementValue(cellIndex, cellVoltage);
				cellVoltagesListResource.notifyListValuesChanged();
			}
		} else if (fileName.startsWith(TEMPERATURE_)) {
			int sensorIndex = Integer.parseInt(fileName.substring(12));
			float sensorTemperature = Float.parseFloat(fileContent);
			temperatures.set(sensorIndex, sensorTemperature);
			if (temperaturesListResource != null) {
				temperaturesListResource.setElementValue(sensorIndex, sensorTemperature);
				temperaturesListResource.notifyListValuesChanged();
			}
		}
	}

	/**
	 * @return the batteryCurrent
	 */
	public synchronized float getBatteryCurrent() {
		return batteryCurrent;
	}

	/**
	 * @return the batteryVoltage
	 */
	public synchronized float getBatteryVoltage() {
		return batteryVoltage;
	}

	/**
	 * @return the batteryPower
	 */
	public synchronized float getBatteryPower() {
		return batteryPower;
	}

	/**
	 * @return the cellVoltageMin
	 */
	public synchronized float getCellVoltageMin() {
		return cellVoltageMin;
	}

	/**
	 * @return the cellVoltageMax
	 */
	public synchronized float getCellVoltageMax() {
		return cellVoltageMax;
	}

	/**
	 * @return the cellVoltageAverage
	 */
	public synchronized float getCellVoltageAverage() {
		return cellVoltageAverage;
	}

	/**
	 * @return the isCharging
	 */
	public synchronized boolean isCharging() {
		return isCharging;
	}

	/**
	 * @return the cellsVoltage
	 */
	public synchronized List<Float> getCellsVoltage() {
		return cellsVoltage;
	}

	/**
	 * @return the soc
	 */
	public synchronized float getSoc() {
		return soc;
	}

	public synchronized float getCellVoltage(int index) {
		return cellsVoltage.get(index) != null ? cellsVoltage.get(index) : 0.0f;
	}

	@Override
	public void startReading() {
		this.start();
	}

	@Override
	public void stopReading() {
		read = false;
	}

	@Override
	public float getTemperature(int index) {
		return temperatures.get(index);
	}
	
	/**
	 * 
	 * @return Default path to files, no trailing backslash.
	 */
	public static String getDefaultPathToFiles() {
		// This function must return the path to files the simulator
		// generates.
		return TMP + SEPARATOR + OUR_FOLDER;
	}
}
