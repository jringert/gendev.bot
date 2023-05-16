package gendev.bot.runner;

import java.io.IOException;
import java.io.InputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;

import gendev.bot.runner.umple.AbstractBot.MotorCmd;

public class ExecutorTcpHandler {

	private Executor ex;
	private RobotRunnerGui gui;

	protected ServerSocket sSock;
	protected Socket cSock;

	public ExecutorTcpHandler(RobotRunnerGui gui, Executor ex) {
		this.gui = gui;
		this.ex = ex;
	}

	public void init(String ipAddr) throws UnknownHostException, IOException {
		sSock = new ServerSocket(5000, 1, InetAddress.getByName(ipAddr));
	}

	public void serveForever() throws IOException {
		while (true) {
			cSock = sSock.accept();
			InputStream inStream = cSock.getInputStream();
			
			System.out.println("connection from " + cSock.getInetAddress());
			
			while (!cSock.isClosed() || !cSock.isInputShutdown()) {
				if(inStream.available() > 0) {
					byte[] buffer = new byte[4096];
					
					System.out.printf("num_avail = %d\n", inStream.available());
					int len = inStream.read(buffer);
					buffer[len] = 0;
					
					
					String msg = new String(buffer, 0, len);
					System.out.println("message_recv = " + msg);

					int dist = Integer.parseInt(msg);
					
					ex.setDistance(dist);
					ex.setDoWork(gui.isWork());
					ex.setEmgOff(gui.isEmgStop());
					ex.nextStep();
					
					String res = cmd2Str(ex.getLeftM()) + "," + cmd2Str(ex.getRightM());
					System.out.println(res);
					cSock.getOutputStream().write(res.getBytes());					
				}
			}
		}
	}

	private String cmd2Str(MotorCmd cmd) {
		return switch (cmd) {
		case BWD:
			yield "B";
		case FWD:
			yield "F";
		case STP:
			yield "S";
		};
	}
}
