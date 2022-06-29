package gendev.bot.runner.aspectj;

import java.io.IOException;
import java.io.OutputStream;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;

import gendev.bot.runner.RobotRunnerGui;
import gendev.bot.runner.umple.AbstractBot.MotorCmd;

public class ExecutorHandler implements HttpHandler {
	private RobotRunnerGui gui;
	
	private int distance;
	private MotorCmd leftM, rightM;

	public ExecutorHandler(RobotRunnerGui gui) {
		this.gui = gui;
	}

	@Override
	public void handle(HttpExchange httpExchange) throws IOException {
		String requestParam = httpExchange.getRequestURI().toString().split("\\?")[1];

		String json = getNextStep(requestParam);

		OutputStream outputStream = httpExchange.getResponseBody();
		httpExchange.sendResponseHeaders(200, json.length());
		outputStream.write(json.getBytes());
		outputStream.flush();
		outputStream.close();
	}

	/**
	 * example requestParam string
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
				if ("distance".equals(var)) {
					setDistance(Integer.parseInt(value));
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		if (!gui.isEmgStop()) {
			if (distance < 20) {
				backing();
			} else if (Math.random() > .5) {
				forwarding();
			} else {
				turning();
			}
		} else {
			stopping();
		}
		
		Map<String, String> sysValuesForEsp = new HashMap<>();
		sysValuesForEsp.put("leftM", leftM.name());
		sysValuesForEsp.put("rightM", rightM.name());
		
		return toJson(sysValuesForEsp);
	}

	private void stopping() {
		leftM = MotorCmd.STP;
		rightM = MotorCmd.STP;		
	}

	private void turning() {
		leftM = MotorCmd.BWD;
		rightM = MotorCmd.FWD;		
	}

	private void forwarding() {
		leftM = MotorCmd.FWD;
		rightM = MotorCmd.FWD;		
	}

	private void backing() {
		leftM = MotorCmd.BWD;
		rightM = MotorCmd.BWD;		
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

	public int getDistance() {
		return distance;
	}

	public void setDistance(int distance) {
		this.distance = distance;
	}

}
