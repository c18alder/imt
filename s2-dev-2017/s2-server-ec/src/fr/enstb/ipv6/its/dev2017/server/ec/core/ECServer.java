package fr.enstb.ipv6.its.dev2017.server.ec.core;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.SocketException;
import java.util.LinkedList;
import java.util.Properties;

import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.network.config.NetworkConfig;

import fr.enstb.ipv6.its.dev2017.server.ec.coap.SimpleCoAPServer;
import fr.enstb.ipv6.its.dev2017.server.ec.resources.KMSResource;
import fr.enstb.ipv6.its.dev2017.server.ec.resources.ShutdownResource;
import fr.enstb.ipv6.its.dev2017.server.ec.resources.SpeedResource;
import fr.enstb.osv.ECDataReader;
import fr.enstb.osvdatasimulator.ec.main.ECDataReaderSimulated;
import fr.enstb.osvinterface.ec.main.ECDataReaderOSV;
import fr.enstb.osvinterface.ec.main.ECDataReaderOSV.ENUM_EC_READER_STATUS;

/**
 * This is the implementation of the server providing the EC resources. The
 * parameters are configurable by tweaking the ECConfig.properties file.
 * 
 * @author Alan Gardin, Anis Hachicha and Abdessalem Khedher
 * @version 1.0
 */
public class ECServer {
	private static int port; // Port on which the server should listen to
	private static boolean simulated = false; // Indicates whether to use the simulator
										// or not
	public static boolean useName = false;
	public static ECDataReader ecDataReader = null;
	static SimpleCoAPServer ecServer = null;

	public static final String CALIFORNIUM_CONFIG_FILE = "/etc/s2-server-ec/Californium.properties";
	public static final String EC_CONFIG_FILE = "/etc/s2-server-ec/ECConfig.properties";

	public static void main(String[] args) throws SocketException {
		do {

			if (ecServer != null) {
				ecServer.stop();
			}

			// Californium config
			if (new File(CALIFORNIUM_CONFIG_FILE).exists()) {
				NetworkConfig.createStandardWithFile(new File(CALIFORNIUM_CONFIG_FILE));
			} else {
				NetworkConfig.createStandardWithFile(new File("Californium.properties"));
			}

			/* Read the config file */
			// TODO get config file from args.
			if (new File(EC_CONFIG_FILE).exists()) {
				readConfig(EC_CONFIG_FILE);
			} else {
				readConfig("ECConfig.properties");
			}
			
			System.out.println("READ CONFIG.");
			
			try {
				Thread.sleep(5000);
			} catch (InterruptedException e) {
			}
			
			System.out.println("SLEEP FINISHED.");

			/* Resources setup */
			SpeedResource speed = new SpeedResource();
			KMSResource kms = new KMSResource();
			ShutdownResource shutdown = new ShutdownResource();

			/* Data reader setup */
			if (simulated) {
				ecDataReader = new ECDataReaderSimulated(speed, kms);
			} else {
				ecDataReader = new ECDataReaderOSV(speed, kms);
				Runtime.getRuntime().addShutdownHook(new Thread() {
					@Override
					public void run() {
						((ECDataReaderOSV) ecDataReader).stopReading();
					}
				});
			}

			System.err.println("EC reader ready.");

			ecDataReader.startReading();

//			while (ECDataReaderOSV.ecDataReaderStatus == ENUM_EC_READER_STATUS.READY) {
//				try {
//					Thread.sleep(500);
//				} catch (InterruptedException e) {
//				}
//			}
//
//			if (ECDataReaderOSV.ecDataReaderStatus == ENUM_EC_READER_STATUS.ERROR
//					|| ECDataReaderOSV.ecDataReaderStatus == ENUM_EC_READER_STATUS.STOPPED) {
//				continue;
//			}

			/* Server setup */
			LinkedList<CoapResource> resources = new LinkedList<CoapResource>();
			resources.add(speed);
			resources.add(kms);
			resources.add(shutdown);

			ecServer = new SimpleCoAPServer(resources, port);
			ecServer.enableLogging(true);

			Runtime.getRuntime().addShutdownHook(new Thread() {
				@Override
				public void run() {
					ecServer.stop();
				}
			});

			try {
				ecServer.start();
			} catch (IllegalStateException e) {
				System.err.println("Failed to start server, please check your network config: " + e.getMessage());
			}
			
			while (ECDataReaderOSV.ecDataReaderStatus == ENUM_EC_READER_STATUS.READY) {
				try {
					Thread.sleep(500);
				} catch (InterruptedException e) {
				}
			}

			if (ECDataReaderOSV.ecDataReaderStatus == ENUM_EC_READER_STATUS.ERROR
					|| ECDataReaderOSV.ecDataReaderStatus == ENUM_EC_READER_STATUS.STOPPED) {
				continue;
			}

			do {
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
				}
			} while (ECDataReaderOSV.ecDataReaderStatus != ENUM_EC_READER_STATUS.STOPPED);

		} while (true);
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
				}
			}
		}
	}
}
