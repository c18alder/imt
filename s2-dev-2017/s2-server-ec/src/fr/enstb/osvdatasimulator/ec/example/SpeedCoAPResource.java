/**
 * 
 */
package fr.enstb.osvdatasimulator.ec.example;

import fr.enstb.osvdatasimulator.ec.network.OSVFloatCoAPResource;

/**
 * @author guillaumelg
 *
 */
public class SpeedCoAPResource implements OSVFloatCoAPResource {

	@Override
	public void setValue(float val) {
		// This is an example...
		System.out.println("Speed changed: " + val + "kms/h.");
		System.out.println("-----------------------------");
	}

}
