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

package fr.enstb.osvdatasimulator.bms.main;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * @author guillaumelg
 *
 */
public class Tools {

	/**
	 * 
	 * @param path Path to file to write to.
	 * @param content String to write in file.
	 */
	public static void writeToFile(String path, String content) {
		BufferedWriter bw = null;
		try {
			bw = new BufferedWriter(new PrintWriter(path));
			bw.write(content + "\n");
		} catch (IOException e) {
			System.err.println("Erreur: " + e.toString());
		} finally {
			if(bw != null) {
				try {
					bw.close();
				} catch (IOException e) {
				}
			}
		}
	}

	public static String readFile(String path) {
		BufferedReader br = null;
		String toReturn = null;
		try {
			br = new BufferedReader(new FileReader(new File(path)));
			toReturn = br.readLine();
		} catch (IOException e) {
			System.err.println("Erreur: " + e.toString());		} finally {
			if(br != null) {
				try {
					br.close();
				} catch (IOException e2) {
				}
			}
		}
		return toReturn;
	}

}
