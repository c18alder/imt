/**
 * This is the implementation of the soc resource. It indicates the current State Of Charge of the battery.
 * 
 * @author Alan Gardin, Anis Hachicha and Abdessalem Khedher
 * @version 1.0
 */

package fr.enstb.ipv6.its.dev2017.server.bms.resources;

import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.server.resources.CoapExchange;
import org.json.simple.JSONObject;

import fr.enstb.ipv6.its.dev2017.server.bms.core.BMSServer;
import fr.enstb.osvdatasimulator.bms.network.OSVFloatCoAPResource;

public class SocResource extends CoapResource implements OSVFloatCoAPResource {
	private float data = 0f;

	public SocResource() {

		// Set resource identifier
		super("soc");

		// Set display name
		getAttributes().setTitle("State Of Charge");

		// Make the resource obervable
		this.setObservable(true);
	}

	@SuppressWarnings("unchecked")
	@Override
	public void handleGET(CoapExchange exchange) {

		// Json conversion
		JSONObject json = new JSONObject();
		if (BMSServer.useName) {
			json.put("name", "State Of Charge");
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