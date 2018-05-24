/**
 * This is the implementation of the average voltage resource.
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

public class CellVoltageAverageResource extends CoapResource implements OSVFloatCoAPResource {
	private float data = 0f;

	public CellVoltageAverageResource() {

		// Set resource identifier
		super("average");

		// Set display name
		getAttributes().setTitle("Cells' average voltage");

		// Make the resource obervable
		this.setObservable(true);

	}

	@SuppressWarnings("unchecked")
	@Override
	public void handleGET(CoapExchange exchange) {

		// Json conversion
		JSONObject json = new JSONObject();
		if (BMSServer.useName) {
			json.put("name", "Cells' average voltage");
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
