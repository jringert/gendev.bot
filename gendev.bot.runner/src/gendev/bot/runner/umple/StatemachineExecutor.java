package gendev.bot.runner.umple;

import java.io.IOException;
import java.io.OutputStream;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;

import gendev.bot.runner.RobotRunnerGui;

public class StatemachineExecutorHandler implements HttpHandler {
	private static AbstractBot bot;
	private RobotRunnerGui gui;
	private boolean prevWork = false;
	private boolean prevEmgStop = false;

	public StatemachineExecutorHandler(RobotRunnerGui gui) {
		if (bot == null) {
			bot = new BotImpl();
		}
		this.gui = gui;
	}

	@Override
	public void handle(HttpExchange httpExchange) throws IOException {
		String requestParam = httpExchange.getRequestURI().toString().split("\\?")[1];
		
		String json = getNextStep(requestParam);
		System.out.println(requestParam + " -> " + json);
		System.out.println(bot);
		
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
		for (String pair : requestParam.split("&")) {
			try {
				String var = pair.split("=")[0];
				String value = pair.split("=")[1];
				if ("distance".equals(var)) {
					bot.setDistance(Integer.parseInt(value));
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		

		// TODO call the actual methods here 
		if (bot.getDistance() < 20) {
			bot.obstacle();
		} else {
			bot.clear();
		}
		
		if (gui.isEmgStop()) {
			bot.emgStop();
		}
		if (gui.isWork()) {
			bot.doWork();
		}

		if (!gui.isEmgStop() && prevEmgStop) {
			bot.releaseEmgStop();
		}
		prevEmgStop = gui.isEmgStop();
		if (!gui.isWork() && prevWork) {
			bot.doneWorking();
		}
		prevWork = gui.isWork();
		
		// assemble output
		Map<String, String> sysValues = new HashMap<>();
		sysValues.put("leftM", bot.getLeftM().toString());
		sysValues.put("rightM", bot.getRightM().toString());
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
