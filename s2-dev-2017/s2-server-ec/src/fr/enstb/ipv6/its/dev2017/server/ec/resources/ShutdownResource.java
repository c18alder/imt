/**
 * 
 */
package fr.enstb.ipv6.its.dev2017.server.ec.resources;

import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.server.resources.CoapExchange;
import org.json.simple.JSONObject;

import fr.enstb.ipv6.its.dev2017.server.ec.core.ECServer;

/**
 * @author guillaumelg
 *
 */
public class ShutdownResource extends CoapResource {
	public ShutdownResource() {

		// Set resource identifier
		super("shutdown");

		// Set display name
		getAttributes().setTitle("Shutdown host");

		// Make the resource obervable
		this.setObservable(false);
	}

	@SuppressWarnings("unchecked")
	@Override
	public void handleGET(CoapExchange exchange) {

		// Json conversion.
		JSONObject json = new JSONObject();
		json.put("state", "running");

		exchange.respond(json.toJSONString());
	}

	@SuppressWarnings("unchecked")
	@Override
	public void handlePOST(CoapExchange exchange) {

		JSONObject json = new JSONObject();
		json.put("state", "halting");

		exchange.respond(json.toJSONString());
		
		ECServer.ecDataReader.shutdownRequest();
	}
}
