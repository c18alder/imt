/**
 * This is the implementation of the cells voltage resource.
 * 
 * @author Alan Gardin, Anis Hachicha and Abdessalem Khedher
 * @version 1.0
 */

package fr.enstb.ipv6.its.dev2017.server.bms.resources;

import java.util.List;
import java.util.ArrayList;

import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.server.resources.CoapExchange;
import org.json.simple.JSONObject;

import fr.enstb.ipv6.its.dev2017.server.bms.core.BMSServer;
import fr.enstb.osvdatasimulator.bms.network.OSVFloatListCoAPResource;

public class CellVoltagesListResource extends CoapResource implements OSVFloatListCoAPResource {
	private List<Float> data;

	public CellVoltagesListResource() {
		// Set resource identifier
		super("cell_voltage");

		// Set display name
		getAttributes().setTitle("List of the cells' voltages");

		this.data = new ArrayList<Float>();
		for (int i = 0; i < 24; i++) { // According to BMSDataReader implementation
			this.data.add((float) 0.0);
		}

		// Make the resource obervable
		this.setObservable(true);
	}

	@SuppressWarnings("unchecked")
	@Override
	public void handleGET(CoapExchange exchange) {

		// Json conversion
		JSONObject json = new JSONObject();
		if (BMSServer.useName) {
			json.put("name", "List of the cells' voltages");
		}
		json.put("values", this.data);

		exchange.respond(json.toJSONString());
	}

	@Override
	public void setElementValue(int index, float value) {
		this.data.remove(index);
		this.data.add(index, value);
		this.changed();
	}

	@Override
	public void notifyListValuesChanged() {
		// TODO Auto-generated method stub

	}
}
