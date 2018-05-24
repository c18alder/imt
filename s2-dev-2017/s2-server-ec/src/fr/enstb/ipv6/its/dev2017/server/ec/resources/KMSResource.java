/**
 * This is the implementation of the KMS resource. It indicates the distance traveled by the vehicle.
 * 
 * @author Alan Gardin, Anis Hachicha and Abdessalem Khedher
 * @version 1.0
 */

package fr.enstb.ipv6.its.dev2017.server.ec.resources;

import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.server.resources.CoapExchange;
import org.json.simple.JSONObject;

import fr.enstb.ipv6.its.dev2017.server.ec.core.ECServer;
import fr.enstb.osvdatasimulator.ec.network.OSVFloatCoAPResource;

public class KMSResource extends CoapResource implements OSVFloatCoAPResource {
	private float data = 0f;

	public KMSResource() {

		// Set resource identifier
		super("kms");

		// Set display name
		getAttributes().setTitle("Vehicle's distance");

		// Make the resource obervable
		this.setObservable(true);
	}

	@SuppressWarnings("unchecked")
	@Override
	public void handleGET(CoapExchange exchange) {

		// Json conversion
		JSONObject json = new JSONObject();
		if (ECServer.useName) {
			json.put("name", "Vehicle's distance");
		}
		json.put("value", this.data);

		exchange.respond(json.toJSONString());
	}

	@Override
	public void setValue(float val) {
		this.data = val;
		this.changed();
	}
}