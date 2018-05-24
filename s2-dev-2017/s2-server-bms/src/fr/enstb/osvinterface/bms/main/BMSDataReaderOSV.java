/**
 * 
 */
package fr.enstb.osvinterface.bms.main;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import fr.enstb.osv.BMSDataReader;
import fr.enstb.osvdatasimulator.bms.network.OSVBooleanCoAPResource;
import fr.enstb.osvdatasimulator.bms.network.OSVFloatCoAPResource;
import fr.enstb.osvdatasimulator.bms.network.OSVFloatListCoAPResource;
import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.UnsupportedCommOperationException;

/**
 * @author guillaumelg
 *
 */
public class BMSDataReaderOSV extends Thread implements BMSDataReader {
	private volatile boolean read = true;

	private static final String BMS_FILE_TTY_ACM0 = "/dev/ttyACM0";
	public static final String BMS_FILE_TTY_S80 = "/dev/ttyS80";
	// private static final String BMS_FILE_TTY_ACM0 = "/dev/pts/2";
	private static final String START_SENDING_DATA_STRING = "01";
	private static final String STOP_SENDING_DATA_STRING = "00\n";
	private static final int NUMBER_OF_CELLS = 24;

	private static final int BAUDRATE = 9600;

	// In ms.
	private static final long READ_TIME = 15000;
	private static final long PAUSE_TIME = 25000;

	SerialPort serialPort = null;

	private float batteryCurrent = 0.0f;
	private float batteryVoltage = 0.0f;
	private float batteryPower = 0.0f;
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

	private BufferedReader in;
	private BufferedWriter out;

	private boolean connected = false;

	public enum ENUM_BMS_READER_STATUS {
		NOT_STARTED, READY, RUNNING, ERROR, STOPPED
	};

	public static volatile ENUM_BMS_READER_STATUS bMSDataReaderStatus = ENUM_BMS_READER_STATUS.NOT_STARTED;

	/**
	 * Default constructor.
	 * 
	 * User must provide references to network resources, so that their values are
	 * updated as soon as possible, when a new value is received. "null" is an
	 * acceptable value. If null reference is provided, nothing will happen on value
	 * changed.
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
	public BMSDataReaderOSV(OSVFloatCoAPResource batteryCurrent, OSVFloatCoAPResource batteryVoltage,
			OSVFloatCoAPResource batteryPower, OSVFloatCoAPResource cellVoltageMin, OSVFloatCoAPResource cellVoltageMax,
			OSVFloatCoAPResource cellVoltageAverage, OSVBooleanCoAPResource isCharging, OSVFloatCoAPResource soc,
			OSVFloatListCoAPResource cellVoltagesList, OSVFloatListCoAPResource temperatureListResource) {

		this.batteryCurrentResource = batteryCurrent;
		this.batteryVoltageResource = batteryVoltage;
		this.batteryPowerResource = batteryPower;
		this.cellVoltageMinResource = cellVoltageMin;
		this.cellVoltageMaxResource = cellVoltageMax;
		this.cellVoltageAverageResource = cellVoltageAverage;
		this.isChargingResource = isCharging;
		this.socResource = soc;
		this.cellVoltagesListResource = cellVoltagesList;
		this.temperaturesListResource = temperatureListResource;

		for (int i = 0; i < NUMBER_OF_CELLS; i++) {
			cellsVoltage.add(0.0f);
		}
		for (int i = 0; i < 4; i++) {
			temperatures.add(0.0f);
		}

		bMSDataReaderStatus = ENUM_BMS_READER_STATUS.READY;
	}

	@Override
	public void run() {
		while (!(new File(BMS_FILE_TTY_ACM0).exists())) {
			if (!doesSymlinkTTYS80HasTarget()) {
				deleteSymlinkTTYS80();
			}
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
			}
		}

		try {
			Runtime.getRuntime().exec("ln -s " + BMS_FILE_TTY_ACM0 + " " + BMS_FILE_TTY_S80);
			if (!(new File(BMS_FILE_TTY_ACM0).exists())) {
				throw new IOException("Failed to create symlink for " + BMS_FILE_TTY_ACM0);
			}
		} catch (IOException e1) {
			bMSDataReaderStatus = ENUM_BMS_READER_STATUS.ERROR;
			e1.printStackTrace();
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
			}
			return;
		}

		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
		}

		System.out.println("Will connect");

		connect(BMS_FILE_TTY_S80);

		System.out.println("Connected");

		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
		}

		if (bMSDataReaderStatus == ENUM_BMS_READER_STATUS.ERROR) {
			System.err.println("Failed to start communication.");
			stopReading();
			return;
		}

		System.out.println("Starting communication.");
		if (out == null) {
			System.err.println("Failed to open serial port!");
			stopReading();
			System.exit(1);
		}

		// Start "read / pause" loop here.
		long pauseReadingTime = 0l;
		while (read) {
			try {
				out.write(START_SENDING_DATA_STRING);
				out.flush();
			} catch (IOException e) {
				bMSDataReaderStatus = ENUM_BMS_READER_STATUS.ERROR;
				System.err.println(e.getMessage());
				stopReading();
				return;
			}

			pauseReadingTime = new Date().getTime() + READ_TIME;

			bMSDataReaderStatus = ENUM_BMS_READER_STATUS.RUNNING;

			if (connected) {
				String readLine;
				boolean pauseRequired = false;
				int i = 0;
				while (read && !pauseRequired) {
					try {
						synchronized (this) {
							readLine = in.readLine();
						}
						processReceivedBMSData(readLine);
					} catch (IOException e) {
						System.err.println(e.toString());
						bMSDataReaderStatus = ENUM_BMS_READER_STATUS.ERROR;
						stopReading();
						return;
					}
					if (++i > 50) {
						if ((new Date().getTime()) > pauseReadingTime) {
							pauseRequired = true;
						} else {
							i = 0;
						}
					}
				}
			}

			try {
				out.write(STOP_SENDING_DATA_STRING);
				out.flush();
			} catch (IOException e) {
				bMSDataReaderStatus = ENUM_BMS_READER_STATUS.ERROR;
				System.err.println(e.getMessage());
				stopReading();
				return;
			}
			try {
				Thread.sleep(PAUSE_TIME);
			} catch (InterruptedException e) {
			}
		} //////////
	}

	private synchronized void processReceivedBMSData(String recvData) {

		if (recvData.length() == 5) {
			int key = Integer.parseInt(recvData.substring(0, 2));
			int value = Integer.parseInt(recvData.substring(2, 5));

			switch (key) {
			case 94:
				int key2 = Integer.parseInt(recvData.substring(2, 3));
				if (key2 == 0 || key2 == 1) {
					setSoc(Integer.parseInt(recvData.substring(2, 5)));
				} else if (key2 == 2) {
					int key3 = Integer.parseInt(recvData.substring(3, 4));
					if (key3 == 1) {
						isCharging(Integer.parseInt(recvData.substring(4, 5)) == 1);
					} else if (key3 == 2) {
						// TODO Is balancing.
					}
				} else if (key2 >= 3) {
					// TODO PWM to charger
				}
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
				setCellVoltage(((float) value) / 200.0f, key - 1);
				break;
			case 74:
				int sensorIndex = Integer.parseInt(recvData.substring(2, 3));
				int senseorTemperature = Integer.parseInt(recvData.substring(3, 5));
				setTemperature(senseorTemperature, sensorIndex);
				break;
			case 84:
				setBatteryCurrent(((float) value) - 400f);
				break;
			case 99:
				// TODO PWM to EC
				break;
			default:
				System.out.println("Received: " + recvData);
			}

		}
	}

	private void connect(String portName) {
		try {
			System.out.println("Will try to get port identifier.");
			CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);
			if (portIdentifier.isCurrentlyOwned()) {
				System.err.println("Error: Port is currently in use");
			} else {
				System.out.println("Port is available");
				CommPort commPort = portIdentifier.open(this.getClass().getName(), 2000);

				if (commPort instanceof SerialPort) {
					System.out.println("Port is serial port");
					serialPort = (SerialPort) commPort;
					serialPort.setSerialPortParams(BAUDRATE, SerialPort.DATABITS_8, SerialPort.STOPBITS_1,
							SerialPort.PARITY_NONE);

					in = new BufferedReader(new InputStreamReader(serialPort.getInputStream()));

					out = new BufferedWriter(new PrintWriter(serialPort.getOutputStream()));

					connected = true;

					System.out.println("Connected.");

					return;

				} else {
					System.err.println("Error: Port is not a SerialPort.");
				}
			}
		} catch (NoSuchPortException | UnsupportedCommOperationException e) {
			System.err.println(e.toString());
			e.printStackTrace();
			System.exit(1);
		} catch (PortInUseException e) {
			System.err.println(e.toString());
			e.printStackTrace();
			System.exit(1);
		} catch (IOException e) {
			System.err.println(e.toString());
			e.printStackTrace();
			System.exit(1);
		}

		bMSDataReaderStatus = ENUM_BMS_READER_STATUS.ERROR;
		connected = false;
	}

	public void startReading() {
		this.start();
	}

	public synchronized void stopReading() {
		read = false;
		bMSDataReaderStatus = ENUM_BMS_READER_STATUS.STOPPED;

		if (out != null) {
			try {
				out.write(STOP_SENDING_DATA_STRING);
				out.flush();
			} catch (IOException e) {
			}
		}
		try {
			if (in != null) {
				in.close();
			}
		} catch (IOException e) {
		}
		try {
			if (out != null) {
				out.close();
			}
		} catch (IOException e) {
		}
		if (serialPort != null) {
			serialPort.close();
		}

		if (!doesSymlinkTTYS80HasTarget()) {
			deleteSymlinkTTYS80();
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
		float min = cellsVoltage.get(0);
		for (Float f : cellsVoltage) {
			if (f < min) {
				min = f;
			}
		}

		return min;
	}

	/**
	 * @return the cellVoltageMax
	 */
	public synchronized float getCellVoltageMax() {
		float max = cellsVoltage.get(0);
		for (Float f : cellsVoltage) {
			if (f > max) {
				max = f;
			}
		}

		return max;
	}

	/**
	 * @return the cellVoltageAverage
	 */
	public synchronized float getCellVoltageAverage() {
		float sum = 0.0f;
		for (Float f : cellsVoltage) {
			sum += f;
		}

		return sum / cellsVoltage.size();
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
		return new ArrayList<Float>(cellsVoltage);
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

	public synchronized float getTemperature(int index) {
		return temperatures.get(index) != null ? temperatures.get(index) : 0.0f;
	}

	/////////////////////////////////////////////

	private synchronized void setBatteryCurrent(float batteryCurrent) {
		if (batteryCurrent != this.batteryCurrent) {
			this.batteryCurrent = batteryCurrent;
			if (batteryCurrentResource != null) {
				batteryCurrentResource.setValue(batteryCurrent);
			}
		}
	}

	private synchronized void setBatteryVoltage(float batteryVoltage) {
		if (batteryVoltage != this.batteryVoltage) {
			this.batteryVoltage = batteryVoltage;
			if (batteryVoltageResource != null) {
				batteryVoltageResource.setValue(batteryVoltage);
			}
		}
	}

	private synchronized void setBatteryPower(float batteryPower) {
		if (batteryPower != this.batteryPower) {
			this.batteryPower = batteryPower;
			if (batteryPowerResource != null) {
				batteryPowerResource.setValue(batteryPower);
			}
		}
	}

	private synchronized void isCharging(boolean charging) {
		if (charging != this.isCharging) {
			this.isCharging = charging;
			if (isChargingResource != null) {
				isChargingResource.setValue(charging);
			}
		}
	}

	private synchronized void setCellVoltage(float voltage, int cellIndex) {
		if (cellsVoltage.get(cellIndex) != voltage) {
			cellsVoltage.set(cellIndex, voltage);

		}
		if (cellIndex == NUMBER_OF_CELLS - 1) {
			if (cellVoltagesListResource != null) {
				for (int i = 0; i < NUMBER_OF_CELLS; i++) {
					cellVoltagesListResource.setElementValue(i, cellsVoltage.get(i));
				}
				cellVoltagesListResource.notifyListValuesChanged();
			}
			// Battery volatge
			float battVoltage = 0.0f;
			for (Float f : cellsVoltage) {
				battVoltage += f;
			}
			setBatteryVoltage(battVoltage);
			setBatteryPower(batteryCurrent * battVoltage);
			setCellMinVoltage();
			setCellMaxVoltage();
			setCellAverageVoltage();
		}
	}

	private void setCellAverageVoltage() {
		float sum = 0.0f;
		for (Float f : cellsVoltage) {
			sum += f;
		}
		if (cellVoltageAverageResource != null) {
			cellVoltageAverageResource.setValue(sum / NUMBER_OF_CELLS);
		}
	}

	private void setCellMaxVoltage() {
		float max = cellsVoltage.get(0);
		for (Float f : cellsVoltage) {
			if (f > max) {
				max = f;
			}
		}
		if (cellVoltageMaxResource != null) {
			cellVoltageMaxResource.setValue(max);
		}
	}

	private void setCellMinVoltage() {
		float min = cellsVoltage.get(0);
		for (Float f : cellsVoltage) {
			if (f < min) {
				min = f;
			}
		}
		if (cellVoltageMinResource != null) {
			cellVoltageMinResource.setValue(min);
		}
	}

	private synchronized void setTemperature(float temperature, int sensorIndex) {
		temperatures.set(sensorIndex, temperature);
		if (temperaturesListResource != null) {
			temperaturesListResource.setElementValue(sensorIndex, temperature);
			temperaturesListResource.notifyListValuesChanged();
		}
	}

	private synchronized void setSoc(float soc) {
		this.soc = soc;
		if (socResource != null) {
			socResource.setValue(soc);
		}
	}

	public static void deleteSymlinkTTYS80() {
		// System.out.println("Will delete symlink.");
		//
		//// ProcessBuilder pb = new ProcessBuilder("/bin/bash", "sudo rm -f",
		// BMS_FILE_TTY_S80);
		// try {
		// Thread.sleep(10);
		//// pb.start();
		// Runtime.getRuntime().exec("/home/user/osv/rmTtyS80.sh");
		// } catch (IOException e) {
		// System.err.println("IOException while removing symlink: " + e.getMessage());
		// } catch (InterruptedException e) {
		// }
	}

	public static boolean doesSymlinkTTYS80HasTarget() {

		if (new File(BMS_FILE_TTY_ACM0).exists()) {
			return true;
		}
		return false;

	}
}
