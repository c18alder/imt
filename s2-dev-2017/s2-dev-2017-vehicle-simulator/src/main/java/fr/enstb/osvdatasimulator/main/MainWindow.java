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

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.File;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

/**
 * @author guillaumelg
 *
 */
public class MainWindow extends JFrame {

	private static final long serialVersionUID = 5057934226934209445L;
	private static MainWindow mainWindowInstance;
	private JPanel rootPane;
	private JTextField pathToFiles;
	private JButton startStop;
	JLabel speedL;
	JLabel socL;

	private VehicleBehaviourManager evBehaviourManager;
	JButton startStopVehicle;
	JButton chargeVehicle;

	private static final String VEHICLE_SPEED = "Vitesse du véhicule : ";
	private static final String VEHICLE_SPEED2 = " km/h.";
	private static final String VEHICLE_SOC = "SOC du véhicule :     ";
	private static final String VEHICLE_SOC2 = " %.";
	private static final String START = "Démarrer";
	private static final String STOP = "Arrêter";
	static final String START_VEHICLE = "Faire rouler le véhicule";
	static final String STOP_VEHICLE = "Arrêter le véhicule";
	public static final String CHARGE_VEHICLE = "Charger le véhicule";
	public static final String STOP_CHARGING_VEHICLE = "Arrêter de charger";

	public MainWindow() {
		setMinimumSize(new Dimension(320, 240));
		addWindowListener(new WindowListener() {
			@Override
			public void windowOpened(WindowEvent e) {
			}

			@Override
			public void windowIconified(WindowEvent e) {
			}

			@Override
			public void windowDeiconified(WindowEvent e) {
			}

			@Override
			public void windowDeactivated(WindowEvent e) {
			}

			@Override
			public void windowClosing(WindowEvent e) {
				onExit();
			}

			@Override
			public void windowClosed(WindowEvent e) {
			}

			@Override
			public void windowActivated(WindowEvent e) {
			}
		});

		rootPane = (JPanel) getContentPane();
		rootPane.setLayout(new BorderLayout());

		makeTopPannel();
		makeCenterPannel();
		makeBottomPannel();

		evBehaviourManager = new VehicleBehaviourManager(this);

		pack();

		mainWindowInstance = this;
	}

	protected void onExit() {
		synchronized (this) {
			if (evBehaviourManager.isRunning()) {
				evBehaviourManager.stop();
			}
		}
		System.exit(0);
	}

	private void makeBottomPannel() {
		JPanel bottomPanel = new JPanel();
		startStop = new JButton(START);
		bottomPanel.add(startStop);
		rootPane.add(bottomPanel, BorderLayout.SOUTH);
		startStop.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if (evBehaviourManager.isRunning()) {
					stop();
				} else {
					start();
				}
			}
		});

		startStopVehicle = new JButton(STOP_VEHICLE);
		startStopVehicle.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				evBehaviourManager.onStartStopVehhicle();
			}
		});
		startStopVehicle.setEnabled(false);
		bottomPanel.add(startStopVehicle);

		chargeVehicle = new JButton(CHARGE_VEHICLE);
		chargeVehicle.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				evBehaviourManager.onChargeVehicle();
			}
		});
		bottomPanel.add(chargeVehicle);
		chargeVehicle.setEnabled(false);
	}

	private void makeCenterPannel() {
		JPanel centerPanel = new JPanel();
		centerPanel.setLayout(new BoxLayout(centerPanel, BoxLayout.PAGE_AXIS));
		;
		JPanel speedP = new JPanel();
		speedL = new JLabel(VEHICLE_SPEED + "       " + VEHICLE_SPEED2);
		speedP.add(speedL);
		centerPanel.add(speedP);
		JPanel socP = new JPanel();
		socL = new JLabel(VEHICLE_SOC + "       " + VEHICLE_SOC2);
		socP.add(socL);
		centerPanel.add(speedP);
		centerPanel.add(socP);

		centerPanel.setBorder(BorderFactory.createTitledBorder("Visualisation"));
		rootPane.add(centerPanel, BorderLayout.CENTER);
	}

	private void makeTopPannel() {
		JPanel confPanel = new JPanel();
		confPanel.setLayout(new BoxLayout(confPanel, BoxLayout.PAGE_AXIS));
		JLabel pathToFilesL = new JLabel("Chemin vers les fichiers :");
		pathToFiles = new JTextField(App.getDefaultPathToFiles(), 50);
		confPanel.add(pathToFilesL);
		confPanel.add(pathToFiles);
		confPanel.setBorder(BorderFactory.createTitledBorder("Configuration"));
		rootPane.add(confPanel, BorderLayout.NORTH);
	}

	public void display() {
		setVisible(true);
	}

	private synchronized void start() {
		if (!(new File(pathToFiles.getText())).isDirectory()) {
			if (!(new File(pathToFiles.getText())).mkdirs()) {
				displayError("Impossible de créer le chemin vers: \"" + pathToFiles.getText()
						+ "\" (et celui-ci n'existe pas).");
				return;
			}
		}
		pathToFiles.setEnabled(false);
		startStopVehicle.setEnabled(true);
		startStop.setText(STOP);
		startStopVehicle.setText(STOP_VEHICLE);
		chargeVehicle.setText(CHARGE_VEHICLE);
		chargeVehicle.setEnabled(false);
		evBehaviourManager.execute();
	}

	private synchronized void stop() {
		evBehaviourManager.stop();
		evBehaviourManager = new VehicleBehaviourManager(this);
		pathToFiles.setEnabled(true);
		startStopVehicle.setEnabled(false);
		startStop.setText(START);
		updateSpeedLabel(0f);
		startStopVehicle.setText(STOP_VEHICLE);
		chargeVehicle.setEnabled(false);
		chargeVehicle.setText(CHARGE_VEHICLE);
	}

	public void updateSpeedLabel(float speed) {
		speedL.setText(
				VEHICLE_SPEED + (speed >= 10 ? (speed >= 100 ? speed : " " + speed) : "  " + speed) + VEHICLE_SPEED2);
	}

	public void updateSocLabel(float soc) {
		socL.setText(VEHICLE_SOC + (soc > 10 ? (soc >= 100 ? soc : " " + soc) : "  " + soc) + VEHICLE_SOC2);
	}

	public static MainWindow getInstance() {
		return mainWindowInstance;
	}

	public void displayError(String string) {
		JOptionPane.showMessageDialog(this, string, "Erreur…", JOptionPane.ERROR_MESSAGE);
	}

	/**
	 * 
	 * @return Path to the data files, no trailing backslash
	 */
	public String getPathToFiles() {
		String path = pathToFiles.getText();
		if (path.endsWith(App.SEPARATOR)) {
			path = path.substring(0, path.length() - 1);
		}
		return path;
	}
}
