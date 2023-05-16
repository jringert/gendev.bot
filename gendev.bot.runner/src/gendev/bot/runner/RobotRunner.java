package gendev.bot.runner;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

import com.sun.net.httpserver.HttpServer;

import gendev.bot.runner.aspectj.JavaAspectExecutor;
import gendev.bot.runner.blockly.BlocklyCodeExecutor;
import gendev.bot.runner.spectra.SymbolicControllerExecutor;
import gendev.bot.runner.umple.StatemachineExecutor;

public class RobotRunner {

	public static String HOST_IP = "192.168.137.1";

	private static boolean USE_HTTP = true;

	private enum Engine {
		Spectra, StateMachine, Aspect, Blockly
	}

	private static Engine engine = Engine.Spectra;

	/**
	 * Starts the GUI, a webserver for the remote robot, and the selected engine
	 * 
	 * @param args
	 * @throws IOException
	 */
	public static void main(String[] args) throws IOException {

		RobotRunnerGui gui = RobotRunnerGui.createAndShowGUI();
		Executor ex = switch (engine) {
		case Spectra:
			yield new SymbolicControllerExecutor();
		case StateMachine:
			yield new StatemachineExecutor();
		case Aspect:
			yield new JavaAspectExecutor();
		case Blockly:
			yield new BlocklyCodeExecutor();
		default:
			throw new IllegalArgumentException("Unexpected value: ");
		};

		if (USE_HTTP) {
			HttpServer server = HttpServer.create(new InetSocketAddress(HOST_IP, 8080), 0);
			ThreadPoolExecutor threadPoolExecutor = (ThreadPoolExecutor) Executors.newFixedThreadPool(1);
			server.createContext("/robot", new ExecutorHttpHandler(gui, ex));
			server.setExecutor(threadPoolExecutor);
			server.start();
		} else {
			ExecutorTcpHandler h = new ExecutorTcpHandler(gui, ex);
			h.init(HOST_IP);
			h.serveForever();
		}

	}

}
