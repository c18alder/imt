/**
 * 
 */
package fr.enstb.osvdatasimulator.bms.network;

/**
 * Network resources MUST implement this interface, so that they are notified by
 * objects reading data from components.
 * 
 * When you instantiate {@link ECDataReader}, make sure to pass instances of
 * CoAPResources implementing this interface, so that they can be notified of
 * value changed.
 * 
 * @author guillaumelg
 *
 */
public interface OSVFloatCoAPResource {

	/**
	 * Set the value of this resource. This is going to be called by
	 * ECDataReader instance.
	 * 
	 * When implementing this method, make sure to call the changed() method of
	 * the CoAPResource, should the resource be observable.
	 * 
	 * @param val
	 *            The new value for this resource.
	 */
	public void setValue(float val);
}
