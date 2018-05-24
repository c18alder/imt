/**
 * 
 */
package fr.enstb.osv;

import java.util.List;

/**
 * @author guillaumelg
 *
 */
public interface BMSDataReader {
	
	public void startReading();

	public void stopReading();
	
	/**
	 * @return the batteryCurrent
	 */
	public float getBatteryCurrent();

	/**
	 * @return the batteryVoltage
	 */
	public float getBatteryVoltage();

	/**
	 * @return the batteryPower
	 */
	public float getBatteryPower();

	/**
	 * @return the cellVoltageMin
	 */
	public float getCellVoltageMin();

	/**
	 * @return the cellVoltageMax
	 */
	public float getCellVoltageMax();

	/**
	 * @return the cellVoltageAverage
	 */
	public float getCellVoltageAverage();

	/**
	 * @return the isCharging
	 */
	public boolean isCharging();

	/**
	 * @return the cellsVoltage
	 */
	public List<Float> getCellsVoltage();

	/**
	 * @return the soc
	 */
	public float getSoc();

	/**
	 * 
	 * Get specific cell voltage
	 * 
	 * @param index Cell number.
	 * @return
	 */
	public float getCellVoltage(int index);
	
	/**
	 * 
	 * Get specific temperature sensor measurement
	 * 
	 * @param index Temperature sensos.
	 * @return
	 */
	public float getTemperature(int index);
	
}
