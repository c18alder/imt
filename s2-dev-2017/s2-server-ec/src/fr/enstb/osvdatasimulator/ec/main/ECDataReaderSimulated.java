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

package fr.enstb.osvdatasimulator.ec.main;

import java.io.File;
import java.io.IOException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardWatchEventKinds;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;
import java.util.List;

import fr.enstb.osv.ECDataReader;
import fr.enstb.osvdatasimulator.ec.example.ExampleEC;
import fr.enstb.osvdatasimulator.ec.network.OSVFloatCoAPResource;

/**
 * @author guillaumelg
 *
 */
public class ECDataReaderSimulated extends Thread implements ECDataReader {

	private volatile boolean read = true;

	private static final String FILE_SPEED = "speed";
	private static final String FILE_TOTAL_DISTANCE = "total_distance";

	private float speed = 0.0f;
	private float totalDistance = 0.0f;

	private OSVFloatCoAPResource speedNetworkResource;
	private OSVFloatCoAPResource distanceNetworkResource;

	/**
	 * Default constructor.
	 * 
	 * User must provide references to network resources, so that their values
	 * are updated as soon as possible, when a new value is received. "null" is
	 * an acceptable value. If null reference is provided, nothing will happen
	 * on value changed.
	 * 
	 * @param speedNetworkResource
	 *            The network resource for speed.
	 * @param distanceNetworkResource
	 */
	public ECDataReaderSimulated(OSVFloatCoAPResource speedNetworkResource, OSVFloatCoAPResource distanceNetworkResource) {
		this.speedNetworkResource = speedNetworkResource;
		this.distanceNetworkResource = distanceNetworkResource;
	}

	@Override
	public void run() {

		// Wait if directory does not exist.
		while (!(new File(ExampleEC.getDefaultPathToFiles())).isDirectory()) {
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
			}
		}

		WatchService watchService;
		try {
			watchService = FileSystems.getDefault().newWatchService();

			Path directory = Paths.get(ExampleEC.getDefaultPathToFiles());
			directory.register(watchService, StandardWatchEventKinds.ENTRY_MODIFY);
			WatchKey key = null;

			while (read) {
				key = watchService.take();
				if (key != null) {
					List<WatchEvent<?>> eventList = key.pollEvents();

					try {
						for (int i = 0; i < eventList.size(); i++) {
							if (eventList.get(i).context().toString().equals(FILE_SPEED)) {
								setSpeed(Float.parseFloat(Tools
										.readFile(ExampleEC.getDefaultPathToFiles() + ExampleEC.SEPARATOR + FILE_SPEED)));
							} else if (eventList.get(i).context().toString().equals(FILE_TOTAL_DISTANCE)) {
								setTotalDistance(Float.parseFloat(Tools.readFile(
										ExampleEC.getDefaultPathToFiles() + ExampleEC.SEPARATOR + FILE_TOTAL_DISTANCE)));
							}
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

	private synchronized void setSpeed(float speed) {
		this.speed = speed;
		if (speedNetworkResource != null) {
			speedNetworkResource.setValue(speed);
		}
	}

	private synchronized void setTotalDistance(float totalDistance) {
		this.totalDistance = totalDistance;
		if (distanceNetworkResource != null) {
			distanceNetworkResource.setValue(totalDistance);
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
	public void stopReading() {
		read = false;
	}

	@Override
	public void shutdownRequest() {
		System.out.println("Shutdown request ignored in simulated mode.");
	}
}
