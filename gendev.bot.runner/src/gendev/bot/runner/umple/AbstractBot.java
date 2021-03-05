package gendev.bot.runner.umple;

import java.awt.Toolkit;

/**
 * events of the SM:
 * <ul>
 * <li>emgStop: button in GUI</li>
 * <li>doWork: button in GUI</li>
 * <li>obstacle: distance < t</li>
 * <li>clear: distance >= t</li>
 * </ul>
 * actions of the SM:
 * <ul>
 * <li>stop: motors stop</li>
 * <li>go: motors forward</li>
 * <li>turn: turns robot left</li>
 * <li>beep: beeps</li>
 * </ul>
 * 
 * @author ringert
 *
 */
public abstract class AbstractBot {

	public enum MotorCmd {
		FWD, BWD, STP
	}

	private int distance;
	private MotorCmd leftM = MotorCmd.FWD;
	private MotorCmd rightM = MotorCmd.FWD;

	/*
	 * Methods for triggers (SM will override these)
	 */

	public boolean doWork() {
		System.out.println("event doWork not handled yet");
		return false;
	}

	public boolean emgStop() {
		System.out.println("event emgStop not handled yet");
		return false;
	}

	public boolean obstacle() {
		System.out.println("event obstacle not handled yet");
		return false;
	}

	public boolean clear() {
		System.out.println("event clear not handled yet");
		return false;
	}

	public boolean releaseEmgStop() {
		System.out.println("event releaseEmgStop not handled yet");
		return false;
	}

	public boolean doneWorking() {
		System.out.println("event doneWorking not handled yet");
		return false;
	}

	/*
	 * Methods for actions on the robot (SM will call these)
	 */

	public void go() {
		this.leftM = MotorCmd.FWD;
		this.rightM = MotorCmd.FWD;
	}

	public void stop() {
		this.leftM = MotorCmd.STP;
		this.rightM = MotorCmd.STP;
	}

	public void turn() {
		this.leftM = MotorCmd.BWD;
		this.rightM = MotorCmd.FWD;
	}

	public void beep() {
		Toolkit.getDefaultToolkit().beep();
	}

	/*
	 * generated getters and setters
	 */

	public int getDistance() {
		return distance;
	}

	public void setDistance(int distance) {
		this.distance = distance;
	}

	public MotorCmd getRightM() {
		return rightM;
	}

	public void setRightM(MotorCmd rightM) {
		this.rightM = rightM;
	}

	public MotorCmd getLeftM() {
		return leftM;
	}

	public void setLeftM(MotorCmd leftM) {
		this.leftM = leftM;
	}

	/*
	 * diagnostics
	 */
	
	abstract public String getStateFullName();

	@Override
	public String toString() {
		return getStateFullName();
	}

}
