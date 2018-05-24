package fr.enstb.ipv6.its.dev2017.server.bms.core;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.SocketException;
import java.util.LinkedList;
import java.util.Properties;

import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.network.config.NetworkConfig;

import fr.enstb.ipv6.its.dev2017.server.bms.coap.SimpleCoAPServer;
import fr.enstb.ipv6.its.dev2017.server.bms.resources.BatteryCurrentResource;
import fr.enstb.ipv6.its.dev2017.server.bms.resources.BatteryPowerResource;
import fr.enstb.ipv6.its.dev2017.server.bms.resources.BatteryVoltageResource;
import fr.enstb.ipv6.its.dev2017.server.bms.resources.CellVoltageAverageResource;
import fr.enstb.ipv6.its.dev2017.server.bms.resources.CellVoltageMaxResource;
import fr.enstb.ipv6.its.dev2017.server.bms.resources.CellVoltageMinResource;
import fr.enstb.ipv6.its.dev2017.server.bms.resources.CellVoltagesListResource;
import fr.enstb.ipv6.its.dev2017.server.bms.resources.IsChargingResource;
import fr.enstb.ipv6.its.dev2017.server.bms.resources.SocResource;
import fr.enstb.ipv6.its.dev2017.server.bms.resources.TemperaturesListResource;
import fr.enstb.osv.BMSDataReader;
import fr.enstb.osvdatasimulator.bms.main.BMSDataReaderSimulated;
import fr.enstb.osvinterface.bms.main.BMSDataReaderOSV;
import fr.enstb.osvinterface.bms.main.BMSDataReaderOSV.ENUM_BMS_READER_STATUS;

/**
 * This is the implementation of the server providing the BMS resources. The
 * parameters are configurable by tweaking the BMSConfig.properties file.
 * 
 * @author Alan Gardin, Anis Hachicha and Abdessalem Khedher
 * @version 1.0
 */
public class BMSServer {
	private static int port;
	private static boolean simulated;
	/**
	 * Should we display resources name in JSON documents. 
	 */
	public static boolean useName = false;

	static BMSDataReader bMSDataReader = null;
	static SimpleCoAPServer bmsServer = null;

	public static final String CALIFORNIUM_CONFIG_FILE = "/etc/s2-server-bms/Californium.properties";
	public static final String BMS_CONFIG_FILE = "/etc/s2-server-bms/BMSConfig.properties";

	public static void main(String[] args) throws SocketException {

		while (true) {

			if (bmsServer != null) {
				bmsServer.stop();
			}
			
			if (!BMSDataReaderOSV.doesSymlinkTTYS80HasTarget()) {
				System.out.println("Should delete symlink.");
				BMSDataReaderOSV.deleteSymlinkTTYS80();
			} else {
				System.out.println("No symlink.");
			}

			// Californium config
			if (new File(CALIFORNIUM_CONFIG_FILE).exists()) {
				NetworkConfig.createStandardWithFile(new File(CALIFORNIUM_CONFIG_FILE));
			} else {
				NetworkConfig.createStandardWithFile(new File("Californium.properties"));
			}

			/* Read the config file */
			// TODO get config file from args.
			if (new File(BMS_CONFIG_FILE).exists()) {
				readConfig(BMS_CONFIG_FILE);
			} else {
				readConfig("BMSConfig.properties");
			}

			/* Resources setup */
			BatteryCurrentResource batteryCurrent = new BatteryCurrentResource();
			BatteryVoltageResource batteryVoltage = new BatteryVoltageResource();
			BatteryPowerResource batteryPower = new BatteryPowerResource();
			CellVoltageMinResource cellVoltageMin = new CellVoltageMinResource();
			CellVoltageMaxResource cellVoltageMax = new CellVoltageMaxResource();
			CellVoltageAverageResource cellVoltageAverage = new CellVoltageAverageResource();
			IsChargingResource isCharging = new IsChargingResource();
			SocResource soc = new SocResource();
			CellVoltagesListResource cellVoltagesList = new CellVoltagesListResource();
			TemperaturesListResource temperaturesList = new TemperaturesListResource();

			/* Data reader setup */
			if (simulated) {
				bMSDataReader = new BMSDataReaderSimulated(batteryCurrent, batteryVoltage, batteryPower, cellVoltageMin,
						cellVoltageMax, cellVoltageAverage, isCharging, soc, cellVoltagesList, temperaturesList);
			} else {
				bMSDataReader = new BMSDataReaderOSV(batteryCurrent, batteryVoltage, batteryPower, cellVoltageMin,
						cellVoltageMax, cellVoltageAverage, isCharging, soc, cellVoltagesList, temperaturesList);

				Runtime.getRuntime().addShutdownHook(new Thread() {
					@Override
					public void run() {
						((BMSDataReaderOSV) bMSDataReader).stopReading();
					}
				});
			}

			System.err.println("BMS reader ready.");

			bMSDataReader.startReading();

			while (BMSDataReaderOSV.bMSDataReaderStatus == ENUM_BMS_READER_STATUS.READY) {
				try {
					Thread.sleep(500);
				} catch (InterruptedException e) {
				}
			}

			if (BMSDataReaderOSV.bMSDataReaderStatus == ENUM_BMS_READER_STATUS.ERROR
					|| BMSDataReaderOSV.bMSDataReaderStatus == ENUM_BMS_READER_STATUS.STOPPED) {
				continue;
			}

			/* Server setup */
			LinkedList<CoapResource> resources = new LinkedList<CoapResource>();

			resources.add(batteryVoltage);
			resources.add(batteryCurrent);
			resources.add(batteryPower);
			resources.add(isCharging);
			resources.add(soc);

			cellVoltagesList.add();
			cellVoltagesList.add(cellVoltageMin);
			cellVoltagesList.add(cellVoltageMax);
			cellVoltagesList.add(cellVoltageAverage);
			resources.add(cellVoltagesList);
			resources.add(temperaturesList);

			bmsServer = new SimpleCoAPServer(resources, port);
			bmsServer.enableLogging(true);
			Runtime.getRuntime().addShutdownHook(new Thread() {
				@Override
				public void run() {
					bmsServer.stop();
				}
			});

			try {
				bmsServer.start();
			} catch (IllegalStateException e) {
				System.err.println("Failed to start server, please check your network config: " + e.getMessage());
			}

			do {
				try {
					Thread.sleep(5000);
				} catch (InterruptedException e) {
				}
			} while (BMSDataReaderOSV.bMSDataReaderStatus != ENUM_BMS_READER_STATUS.STOPPED);

		}
	}

	/**
	 * Read a config file and update the port of the server and whether or not
	 * the date should be simulated.
	 * 
	 * @param path
	 *            to the file. If the file is placed in the same directory as
	 *            the application, path should be the name of the file.
	 */
	public static void readConfig(String path) {
		Properties prop = new Properties();
		InputStream input = null;

		try {

			input = new FileInputStream(path);
			prop.load(input);

			port = Integer.parseInt(prop.getProperty("PORT"));
			simulated = Boolean.parseBoolean(prop.getProperty("SIMULATED"));
			useName = Boolean.parseBoolean(prop.getProperty("USE_NAME"));

		} catch (IOException ex) {
			ex.printStackTrace();
		} finally { // Close correctly the input
			if (input != null) {
				try {
					input.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
}
