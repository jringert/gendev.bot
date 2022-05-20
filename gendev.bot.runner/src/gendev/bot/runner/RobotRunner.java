package gendev.bot.runner;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

import com.sun.net.httpserver.HttpServer;

import gendev.bot.runner.spectra.SymbolicControllerExecutorHandler;
import gendev.bot.runner.umple.StatemachineExecutorHandler;

public class RobotRunner {
    
  public static String HOST_IP = "192.168.137.1"; 

	private enum Engine {
		Spectra, StateMachine
	}

	private static Engine engine = Engine.StateMachine;

	/**
	 * Starts the GUI, a webserver for the remote robot, and the selected engine
	 * 
	 * @param args
	 * @throws IOException
	 */
	public static void main(String[] args) throws IOException {
//	  disablePrint();

		HttpServer server = HttpServer.create(new InetSocketAddress(HOST_IP, 8080), 0);
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

    private static void disablePrint() {
        System.setOut(new java.io.PrintStream(new java.io.OutputStream() {
            @Override public void write(int b) {}
        }) {
            @Override public void flush() {}
            @Override public void close() {}
            @Override public void write(int b) {}
            @Override public void write(byte[] b) {}
            @Override public void write(byte[] buf, int off, int len) {}
            @Override public void print(boolean b) {}
            @Override public void print(char c) {}
            @Override public void print(int i) {}
            @Override public void print(long l) {}
            @Override public void print(float f) {}
            @Override public void print(double d) {}
            @Override public void print(char[] s) {}
            @Override public void print(String s) {}
            @Override public void print(Object obj) {}
            @Override public void println() {}
            @Override public void println(boolean x) {}
            @Override public void println(char x) {}
            @Override public void println(int x) {}
            @Override public void println(long x) {}
            @Override public void println(float x) {}
            @Override public void println(double x) {}
            @Override public void println(char[] x) {}
            @Override public void println(String x) {}
            @Override public void println(Object x) {}
            @Override public java.io.PrintStream printf(String format, Object... args) { return this; }
            @Override public java.io.PrintStream printf(java.util.Locale l, String format, Object... args) { return this; }
            @Override public java.io.PrintStream format(String format, Object... args) { return this; }
            @Override public java.io.PrintStream format(java.util.Locale l, String format, Object... args) { return this; }
            @Override public java.io.PrintStream append(CharSequence csq) { return this; }
            @Override public java.io.PrintStream append(CharSequence csq, int start, int end) { return this; }
            @Override public java.io.PrintStream append(char c) { return this; }
        });        
    }
}
