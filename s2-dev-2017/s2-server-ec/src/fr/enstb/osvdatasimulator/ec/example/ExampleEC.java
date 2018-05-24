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

package fr.enstb.osvdatasimulator.ec.example;

import fr.enstb.osv.ECDataReader;
import fr.enstb.osvdatasimulator.ec.main.ECDataReaderSimulated;
import fr.enstb.osvdatasimulator.ec.network.OSVFloatCoAPResource;

/**
 * @author guillaumelg
 *
 */
public class ExampleEC {

	public static final String SEPARATOR = System.getProperty("file.separator");
	public static final String TMP = System.getProperty("java.io.tmpdir");
	public static final String OUR_FOLDER = "osvdatasimulator";

	static ECDataReader ecDataReader;
	
	/**
	 * 
	 * This is an example of how this program can be used to read data from the
	 * vehicle.
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		// 1) Instantiate Objects implementing OSVFloatCoAPResource. In real
		// server code, those will also extend CoAPResource.
		OSVFloatCoAPResource speedResource = new SpeedCoAPResource();
		OSVFloatCoAPResource distanceResource = new DistanceCoAPResource();

		// 2) Instantiate the data reader.
//		ECDataReaderSimulated ecDataReader = new ECDataReaderSimulated(speedResource, distanceResource);
		ecDataReader = new ECDataReaderSimulated(speedResource, distanceResource);
		
		Runtime.getRuntime().addShutdownHook(new Thread() {
			public void run() {
				ecDataReader.stopReading();
			}
		});
		
		ecDataReader.startReading();

		while (true) {
			// 3) Request data, periodically.
			System.out.println("Vehicle speed    (standard method) = " + ecDataReader.getSpeed() + "kms/h.");

			System.out.println("Vehicle distance (standard method) = " + ecDataReader.getTotalDistance() + "kms.");

			System.out.println("-----------------------------");

			try {
				Thread.sleep(10000);
			} catch (InterruptedException e) {
			}
		}
	}

	/**
	 * 
	 * @return Default path to files, no trailing backslash.
	 */
	public static String getDefaultPathToFiles() {
		// TODO This function must return the path to files the simulator
		// generates.
		return TMP + SEPARATOR + OUR_FOLDER;
	}
}
