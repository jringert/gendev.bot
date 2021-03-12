package gendev.bot.runner;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

import com.sun.net.httpserver.HttpServer;

import gendev.bot.runner.spectra.SymbolicControllerExecutorHandler;
import gendev.bot.runner.umple.StatemachineExecutorHandler;

public class RobotRunner {

	private enum Engine {
		Spectra, StateMachine
	}

	private static Engine engine = Engine.Spectra;

	/**
	 * Starts the GUI, a webserver for the remote robot, and the selected engine
	 * 
	 * @param args
	 * @throws IOException
	 */
	public static void main(String[] args) throws IOException {

		HttpServer server = HttpServer.create(new InetSocketAddress("192.168.0.11", 8080), 0);
		ThreadPoolExecutor threadPoolExecutor = (ThreadPoolExecutor) Executors.newFixedThreadPool(1);

		RobotRunnerGui gui = RobotRunnerGui.createAndShowGUI();

		switch (engine) {
		case Spectra:
			server.createContext("/robot", new SymbolicControllerExecutorHandler(gui));
			break;
		case StateMachine:
			server.createContext("/robot", new StatemachineExecutorHandler(gui));
			break;
		default:
			break;
		}

		server.setExecutor(threadPoolExecutor);
		server.start();
	}
}
