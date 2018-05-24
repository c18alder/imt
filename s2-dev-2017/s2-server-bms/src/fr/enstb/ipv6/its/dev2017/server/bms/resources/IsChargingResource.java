/**
 * This is the implementation of the charging resource.
 * 
 * @author Alan Gardin, Anis Hachicha and Abdessalem Khedher
 * @version 1.0
 */

package fr.enstb.ipv6.its.dev2017.server.bms.resources;

import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.server.resources.CoapExchange;
import org.json.simple.JSONObject;

import fr.enstb.ipv6.its.dev2017.server.bms.core.BMSServer;
import fr.enstb.osvdatasimulator.bms.network.OSVBooleanCoAPResource;

public class IsChargingResource extends CoapResource implements OSVBooleanCoAPResource {
	private boolean data = false;

	public IsChargingResource() {

		// Set resource identifier
		super("charging");

		// Set display name
		getAttributes().setTitle("Is the system charging?");

		// Make the resource obervable
		this.setObservable(true);

	}

	@SuppressWarnings("unchecked")
	@Override
	public void handleGET(CoapExchange exchange) {

		// Json conversion
		JSONObject json = new JSONObject();
		if (BMSServer.useName) {
			json.put("name", "Is the battery charging");
		}
		json.put("value", this.data);
		exchange.respond(json.toJSONString());
	}

	@Override
	public void setValue(boolean val) {
		this.data = val;
		this.changed();
	}
}