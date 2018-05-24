/**
 * 
 */
package fr.enstb.osvdatasimulator.ec.example;

import fr.enstb.osvdatasimulator.ec.network.OSVFloatCoAPResource;

/**
 * @author guillaumelg
 *
 */
public class DistanceCoAPResource implements OSVFloatCoAPResource {

	/* (non-Javadoc)
	 * @see fr.enstb.osvdatasimulator.ec.network.OSVFloatCoAPResource#setValue(float)
	 */
	@Override
	public void setValue(float val) {
		// This is an example...
		System.out.println("Distance changed: " + val + "kms.");
		System.out.println("-----------------------------");
	}

}
