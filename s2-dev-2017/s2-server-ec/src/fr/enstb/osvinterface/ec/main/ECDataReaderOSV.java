/**
 * Copyright 2017 Institut Mines-Télécom
 *
 * This file is part of s2-dev-2017-engine-controller-simulator.
 *
 * s2-dev-2017-engine-controller-simulator is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * s2-dev-2017-engine-controller-simulator is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with s2-dev-2017-engine-controller-simulator. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

package fr.enstb.osvinterface.ec.main;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.util.Date;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import java.util.logging.Level;
import java.util.logging.Logger;

import fr.enstb.osv.ECDataReader;
import fr.enstb.osvdatasimulator.ec.network.OSVFloatCoAPResource;
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
public class ECDataReaderOSV extends Thread implements ECDataReader {

	private volatile boolean read = true;

	private static final String EC_FILE_TTY_USB0 = "/dev/ttyUSB0";
	private static final String EC_FILE_TTY_ACM0 = "/dev/ttyACM0";
	private static final int BAUDRATE = 38400;
	private static final byte[] SPEED_COMMAND = { (byte) 0x53, (byte) 0x04, (byte) 0x03, (byte) 0x05, (byte) 0x16,
			(byte) 0x00, (byte) 0xe3, (byte) 0x9d };
	private static final byte[] DISTANCE_COMMAND = { (byte) 0x53, (byte) 0x04, (byte) 0x03, (byte) 0x15, (byte) 0x4b,
			(byte) 0x00, (byte) 0xda, (byte) 0xc8 };
	private static final int SPEED_COMMAND_RESPONSE_SIZE = 22;
	private static final int DISTANCE_COMMAND_RESPONSE_SIZE = 54;

	private SerialPort serialPort;

	private volatile float speed = 0.0f;
	private volatile float totalDistance = 0.0f;

	private OSVFloatCoAPResource speedNetworkResource;
	private OSVFloatCoAPResource distanceNetworkResource;

	private InputStream in;
	private OutputStream out;

	private boolean connected = false;

	public enum ENUM_EC_READER_STATUS {
		NOT_STARTED, READY, RUNNING, ERROR, STOPPED
	};

	public static volatile ENUM_EC_READER_STATUS ecDataReaderStatus = ENUM_EC_READER_STATUS.NOT_STARTED;

	/**
	 * Default constructor.
	 * 
	 * User must provide references to network resources, so that their values are
	 * updated as soon as possible, when a new value is received. "null" is an
	 * acceptable value. If null reference is provided, nothing will happen on value
	 * changed.
	 * 
	 * @param speedNetworkResource
	 *            The network resource for speed.
	 * @param distanceNetworkResource
	 */
	public ECDataReaderOSV(OSVFloatCoAPResource speedNetworkResource, OSVFloatCoAPResource distanceNetworkResource) {
		this.speedNetworkResource = speedNetworkResource;
		this.distanceNetworkResource = distanceNetworkResource;
		ecDataReaderStatus = ENUM_EC_READER_STATUS.READY;
	}

	@Override
	public void run() {

		// Wait if directory does not exist.
		while (!(new File(EC_FILE_TTY_USB0).exists())) {
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
			}
		}

		while (!(new File(EC_FILE_TTY_ACM0).exists())) {
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
			}
		}

		try {
			Thread.sleep(6000);
		} catch (InterruptedException e) {
		}

		connect(EC_FILE_TTY_USB0);

		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
		}

		if (ecDataReaderStatus == ENUM_EC_READER_STATUS.ERROR) {
			System.err.println("Failed to start communication.");
			stopReading();
			return;
		}

		ecDataReaderStatus = ENUM_EC_READER_STATUS.RUNNING;

		long readTime, sleepTime, timeToSleep;

		if (connected) {
			// WatchService watchService;
			while (read) {

				try {
					if (!new File(EC_FILE_TTY_ACM0).exists()) {
						stopReading();
						return;
					}

					readTime = new Date().getTime();

					readSpeedFromSerial();

					Thread.sleep(20);

					readDistanceFromSerial();

					if (speed > 0.5f) {
						sleepTime = new Date().getTime();

						timeToSleep = (200l - (sleepTime - readTime));

						if (timeToSleep < 150l) {
							timeToSleep = 150l;
						}
					} else {
						timeToSleep = 2000l;
					}
					Thread.sleep(timeToSleep);

				} catch (OSVInterfaceException | InterruptedException e) {
					System.err.println(e.toString());
					ecDataReaderStatus = ENUM_EC_READER_STATUS.ERROR;
					stopReading();
				}
			}
		}
	}

	private void setSpeed(float speed) {
		if (this.speed != speed) {
			this.speed = speed;
			if (speedNetworkResource != null) {
				speedNetworkResource.setValue(speed);
			}
		}
	}

	private void setTotalDistance(float totalDistance) {
		if (this.totalDistance != totalDistance) {
			this.totalDistance = totalDistance;
			if (distanceNetworkResource != null) {
				distanceNetworkResource.setValue(totalDistance);
			}
		}
	}

	@Override
	public synchronized void startReading() {
		this.start();
	}

	@Override
	public synchronized float getSpeed() {
		return speed;
	}

	@Override
	public synchronized float getTotalDistance() {
		return totalDistance;
	}

	@Override
	public synchronized void stopReading() {
		read = false;
		ecDataReaderStatus = ENUM_EC_READER_STATUS.STOPPED;
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
		System.out.println("Closing port");
	}

	private void connect(String portName) {
		try {
			CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);
			if (portIdentifier.isCurrentlyOwned()) {
				System.out.println("Error: Port is currently in use");
			} else {
				System.out.println("Port is available");
				CommPort commPort = portIdentifier.open(this.getClass().getName(), 2000);

				if (commPort instanceof SerialPort) {
					System.out.println("Port is serial port");
					serialPort = (SerialPort) commPort;
					serialPort.setSerialPortParams(BAUDRATE, SerialPort.DATABITS_8, SerialPort.STOPBITS_1,
							SerialPort.PARITY_NONE);

					in = serialPort.getInputStream();

					out = serialPort.getOutputStream();

					connected = true;

					System.out.println("Connected.");

					return;

				} else {
					System.out.println("Port is not a serial port.");
				}
			}
		} catch (NoSuchPortException | UnsupportedCommOperationException e) {
			System.err.println(e.toString());
		} catch (PortInUseException e) {
			System.err.println(e.toString());
		} catch (IOException e) {
			System.err.println(e.toString());
		}

		ecDataReaderStatus = ENUM_EC_READER_STATUS.ERROR;
		connected = false;
	}

	/**
	 * Write an array of bytes to serial port.
	 * 
	 * @param message
	 *            The byte array containing the message to be sent.
	 * @param len
	 *            The number of bytes to be written.
	 * @throws OSVInterfaceException
	 *             If write fails.
	 */
	private void writeToStream(byte[] message, int len) throws OSVInterfaceException {
		if (!connected) {
			throw new OSVInterfaceException("Failed to write: Trying to write to serial while not connected.");
		}
		try {
			// int c = 0;
			// while ((c = System.in.read()) > -1) {
			this.out.write(message, 0, len);
			// }
		} catch (IOException e) {
			throw new OSVInterfaceException("Failed to write: " + e.getMessage());
		}
	}

	/**
	 * Read a number of bytes from the serial interface.
	 * 
	 * @param len
	 *            Number of bytes to be read.
	 * @return An array of size len containing the bytes that have been read.
	 * @throws OSVInterfaceException
	 *             If read fails.
	 */
	private byte[] readFromStream(int len) throws OSVInterfaceException {
		if (!connected) {
			throw new OSVInterfaceException("Failed to read: Trying to read from serial while not connected.");
		}

		byte[] buffer = new byte[len];

		int currentByte = -1, bytesRead = 0;

		try {
			for (int i = 0; i < len; i++) {

				//////////////

				// int readByte = 1;
				// Read data with timeout
				Callable<Integer> readTask = new Callable<Integer>() {
					@Override
					public Integer call() throws Exception {
						return in.read();
					}
				};
				// while (readByte >= 0) {
				ExecutorService executor = Executors.newSingleThreadExecutor();
				Future<Integer> future = executor.submit(readTask);
				currentByte = future.get(200, TimeUnit.MILLISECONDS);
				// if (readByte >= 0) {
				// System.out.println("Read: " + readByte);
				// }
				// }

				//////////////

				// currentByte = this.in.read();

				if (currentByte != -1) {
					buffer[i] = (byte) currentByte;
					bytesRead++;
				} else {
					System.err.println("Failed to read byte from serial.");
					break;
				}
			}
		} catch (InterruptedException e) {
			throw new OSVInterfaceException("Failed to read: " + e.getMessage());
		} catch (ExecutionException e) {
			throw new OSVInterfaceException("Failed to read: " + e.getMessage());
		} catch (TimeoutException e) {
			throw new OSVInterfaceException("Failed to read: " + e.getMessage());
		}
		// catch (IOException e) {
		// throw new OSVInterfaceException("Failed to read: " + e.getMessage());
		// }

		if (bytesRead != len) {
			for (int i = 0; i < bytesRead; i++) {
				System.err.println("recv: " + buffer[i]);
			}
			throw new OSVInterfaceException("Failed to read: got only " + bytesRead + " bytes, instead of " + len);
		}

		return buffer;
	}

	private synchronized void readSpeedFromSerial() throws OSVInterfaceException {
		writeToStream(SPEED_COMMAND, SPEED_COMMAND.length);
		byte[] response = readFromStream(SPEED_COMMAND_RESPONSE_SIZE);

		byte[] result = { (byte) 0x00, (byte) 0x00, response[12], response[13] };

		int val = ByteBuffer.wrap(result).getInt();

		setSpeed(((float) val) / 10f);
	}

	private synchronized void readDistanceFromSerial() throws OSVInterfaceException {
		writeToStream(DISTANCE_COMMAND, DISTANCE_COMMAND.length);
		byte[] response = readFromStream(DISTANCE_COMMAND_RESPONSE_SIZE);

		byte[] result = { (byte) 0x00, response[9], response[10], response[11] };

		int val = ByteBuffer.wrap(result).getInt();

		setTotalDistance(((float) val) / 100f);
	}

	@Override
	public void shutdownRequest() {
		// TODO Auto-generated method stub
		stopReading();

		try {
			Runtime.getRuntime().exec("halt");
			Runtime.getRuntime().exec("echo test > /tmp/test");
			Logger.getGlobal().log(Level.WARNING, "In shutdown request!");

			System.err.println("halt command requested");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	// private static String byteArrayToHex(byte[] a) {
	// StringBuilder sb = new StringBuilder(a.length * 2);
	// for (byte b : a)
	// sb.append(String.format("%02x ", b));
	// return sb.toString();
	// }
}
