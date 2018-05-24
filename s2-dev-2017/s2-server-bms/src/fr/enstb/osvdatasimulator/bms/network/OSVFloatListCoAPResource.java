/**
 * 
 */
package fr.enstb.osvdatasimulator.bms.network;

/**
 * Network resources MUST implement this interface, so that they are notified by
 * objects reading data from components.
 * 
 * When you instantiate {@link BMSDataReaderOSV}, make sure to pass instances of
 * CoAPResources implementing this interface, so that they can be notified of
 * value changed.
 * 
 * @author guillaumelg
 *
 */
public interface OSVFloatListCoAPResource {

	/**
	 * Set the value of one element in this List resource. This is going to be
	 * called by BMSDataReader instance.
	 * 
	 * When implementing this method, make sure to call the changed() method of
	 * the CoAPResource, should the resource be observable.
	 * 
	 * @param index
	 *            The item index in the list of values.
	 * @param value
	 *            The new value for the list item in this resource.
	 */
	public void setElementValue(int index, float value);

	/**
	 * This is called when part or all the list has been modified significantly.
	 * This method should be used as a starting point to notify observers, when
	 * using CoAP.
	 */
	public void notifyListValuesChanged();
}
