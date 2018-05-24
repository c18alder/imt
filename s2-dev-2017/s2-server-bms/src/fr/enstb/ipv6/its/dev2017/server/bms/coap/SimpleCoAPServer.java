/**
 * This is a simple implementation of a CoAP server using the Californium library.
 * This class is a bridge between the Californium library and a more high-level approach to CoAP.
 * 
 * @author Alan Gardin, Anis Hachicha and Abdessalem Khedher
 * @version 1.0
 */

package fr.enstb.ipv6.its.dev2017.server.bms.coap;

import java.io.IOException;
import java.net.BindException;
import java.net.Inet6Address;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.List;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.eclipse.californium.core.CoapClient;
import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.CoapServer;
import org.eclipse.californium.core.Utils.DaemonThreadFactory;
import org.eclipse.californium.core.network.CoapEndpoint;
import org.eclipse.californium.core.network.Endpoint;
import org.eclipse.californium.core.network.EndpointManager;
import org.eclipse.californium.core.network.EndpointManager.ClientMessageDeliverer;
import org.eclipse.californium.core.network.EndpointObserver;

public class SimpleCoAPServer extends CoapServer {
	private final int port; // The port is not expected to change during the use.
	private final static Logger LOGGER = Logger.getLogger(CoapClient.class.getCanonicalName());

	/**
	 * @param resources
	 *            a list of resources to be added to the server.
	 * @param port
	 *            the port on which the server must listen to clients.
	 * @throws SocketException
	 *             in case of network error
	 */
	public SimpleCoAPServer(List<CoapResource> resources, int port) throws SocketException {
		this.port = port;
		this.addResources(resources);
		// this.addEndpoints();
		initCoapLocalEndpoint();
	}

	/**
	 * Enables or disables logging
	 * 
	 * @param state
	 *            is true to enable logging and false otherwise.
	 */
	public void enableLogging(boolean state) {
		if (state) {
			LOGGER.setLevel(Level.ALL);
		} else {
			LOGGER.setLevel(Level.OFF);
		}
	}

	/**
	 * Add the list of ressources to the server
	 * 
	 * @param resources
	 *            a list of CoapResources
	 */
	private void addResources(List<CoapResource> resources) {
		for (CoapResource resource : resources) {
			add(resource);
		}
	}

	/**
	 * Set the listening addresses and the interfaces concerned.
	 * 
	 * @throws SocketException
	 *             in case of network error
	 */
	private void addEndpoints() throws SocketException {
		for (InetAddress addr : EndpointManager.getEndpointManager().getNetworkInterfaces()) {
			// Only binds to IPv6 addresses
			if (addr instanceof Inet6Address) {
				InetSocketAddress bindToAddress = new InetSocketAddress(addr, this.port);
				addEndpoint(new CoapEndpoint(bindToAddress));
			}
		}
	}

	/**
	 * Return the port on which the server is currently listening.
	 * 
	 * @return the current port of the server
	 */
	public int getPort() {
		return this.port;
	}

	public void initCoapLocalEndpoint() {

		final ScheduledExecutorService executor = Executors.newScheduledThreadPool(2,
				new DaemonThreadFactory("EC-executors"));

		InetAddress localhost;
		try {
			localhost = InetAddress.getByName("[::]");
		} catch (UnknownHostException e1) {
			// Fatal crash when not able to resolve localhost address ?
			e1.printStackTrace();
			System.exit(1);
			return;
		}

		try {
			InetSocketAddress address = new InetSocketAddress(localhost, this.port);
			Endpoint customEndpoint = new CoapEndpoint(address);
			customEndpoint.setMessageDeliverer(new ClientMessageDeliverer());
			customEndpoint.setExecutor(executor);
			customEndpoint.addObserver(new EndpointObserver() {
				public void started(Endpoint endpoint) {
				}

				public void stopped(Endpoint endpoint) {
				}

				public void destroyed(Endpoint endpoint) {
					executor.shutdown();
				}
			});

			// EndpointManager.getEndpointManager().setDefaultEndpoint(customEndpoint);
			this.addEndpoint(customEndpoint);
			// try {
			customEndpoint.start();
			System.out.println("Created default endpoint " + customEndpoint.getAddress());
		} catch (BindException e) {
			System.err.println("Bind impossible" + e.getMessage());
		} catch (IOException e) {
			System.err.println("Could not create default endpoint" + e.getMessage());
			// customEndpoint = null;
		}
	}
}