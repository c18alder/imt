/**
 * 
 */
package fr.enstb.osv;

/**
 * @author guillaumelg
 *
 */
public interface ECDataReader {
	public float getSpeed();

	public float getTotalDistance();

	public void startReading();
	
	public void stopReading();
	
	public void shutdownRequest();
}
