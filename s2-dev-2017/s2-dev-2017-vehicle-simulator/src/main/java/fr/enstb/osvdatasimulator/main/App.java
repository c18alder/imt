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

import javax.swing.SwingUtilities;

/**
 * @author guillaumelg
 *
 */
public class App {

	public static final String SEPARATOR = System.getProperty("file.separator");
	public static final String TMP = System.getProperty("java.io.tmpdir");
	public static final String OUR_FOLDER = "osvdatasimulator";
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				(new MainWindow()).display();
			}
		});
	}

	/**
	 * 
	 * @return Default path to files, no trailing backslash.
	 */
	public static String getDefaultPathToFiles() {
		return TMP + SEPARATOR + OUR_FOLDER;
	}
}
