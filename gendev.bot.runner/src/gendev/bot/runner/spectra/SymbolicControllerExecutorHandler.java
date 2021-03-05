package gendev.bot.runner.spectra;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Iterator;
import java.util.Map;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;

import gendev.bot.runner.RobotRunnerGui;
import tau.smlab.syntech.executor.ControllerExecutor;
import tau.smlab.syntech.executor.ControllerExecutorException;

public class SymbolicControllerExecutorHandler implements HttpHandler {
	private ControllerExecutor executor;
	private RobotRunnerGui gui;

	public SymbolicControllerExecutorHandler(RobotRunnerGui gui) {
		this.gui = gui;
		executor = new ControllerExecutor(true, true);
	}

	@Override
	public void handle(HttpExchange httpExchange) throws IOException {
		String requestParam = httpExchange.getRequestURI().toString().split("\\?")[1];

		String json = getNextStep(requestParam);
		System.out.println(requestParam + " -> " + json);

		OutputStream outputStream = httpExchange.getResponseBody();
		httpExchange.sendResponseHeaders(200, json.length());
		outputStream.write(json.getBytes());
		outputStream.flush();
		outputStream.close();
	}

	/**
	 * example requestParam string lightLeft=true&lightRight=false
	 * 
	 * @param requestParam
	 * 
	 * @return a JSON string of the output vars
	 */
	protected String getNextStep(String requestParam) {
		try {
			for (String pair : requestParam.split("&")) {
				String var = pair.split("=")[0];
				String value = pair.split("=")[1];
				executor.setInputValue(var, value);
			}
			// INFO this is somewhat ugly, but simple to add additional input for the
			// controller from the GUI
			executor.setInputValue("emgStop", Boolean.toString(gui.isEmgStop()));
			executor.setInputValue("work", Boolean.toString(gui.isWork()));
		} catch (Exception e) {
			e.printStackTrace();
		}

		try {
			executor.updateState(true);
		} catch (ControllerExecutorException e) {
			e.printStackTrace();
		}

		Map<String, String> sysValues = executor.getCurOutputs();
		return toJson(sysValues);
	}

	protected String toJson(Map<String, String> map) {
		String json = "{";
		for (Iterator<String> it = map.keySet().iterator(); it.hasNext();) {
			String key = it.next();
			json += "\"" + key + "\" : \"" + map.get(key) + "\"";
			if (it.hasNext()) {
				json += ", ";
			}
		}
		json += "}";
		return json;
	}

}
