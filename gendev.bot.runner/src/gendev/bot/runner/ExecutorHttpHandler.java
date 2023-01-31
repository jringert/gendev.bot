package gendev.bot.runner;

import java.io.IOException;
import java.io.OutputStream;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;

public class ExecutorHttpHandler implements HttpHandler {

	private Executor ex;
	private RobotRunnerGui gui;

	public ExecutorHttpHandler(RobotRunnerGui gui, Executor ex) {
		this.gui = gui;
		this.ex = ex;
	}

	@Override
	public void handle(HttpExchange httpExchange) throws IOException {
		String requestParam = httpExchange.getRequestURI().toString().split("\\?")[1];

		try {
			for (String pair : requestParam.split("&")) {
				String var = pair.split("=")[0];
				String value = pair.split("=")[1];
				setField(var, value);
			}
			ex.setEmgOff(gui.isEmgStop());
			ex.setDoWork(gui.isWork());
		} catch (Exception e) {
			e.printStackTrace();
		}

		ex.nextStep();

		String json = assembleJson();

		OutputStream outputStream = httpExchange.getResponseBody();
		httpExchange.sendResponseHeaders(200, json.length());
		outputStream.write(json.getBytes());
		outputStream.flush();
		outputStream.close();
	}

	private String assembleJson() {
		Map<String, String> sysValues = new HashMap<>();
		sysValues.put("leftM", ex.getLeftM().toString());
		sysValues.put("rightM", ex.getRightM().toString());
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

	/**
	 * set the corresponding field in the executor
	 * 
	 * @param name
	 * @param value
	 */
	private void setField(String name, String value) {
		switch (name) {
		case "distance":
			ex.setDistance(Integer.parseInt(value));
			break;
		default:
			return;
		}
	}
}
