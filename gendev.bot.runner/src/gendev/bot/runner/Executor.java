package gendev.bot.runner;

import gendev.bot.runner.umple.AbstractBot.MotorCmd;

public abstract class Executor {

	private int distance;
	private MotorCmd leftM, rightM;
	private boolean emgOff, doWork;
	
	abstract public void nextStep(); 

	public boolean isEmgOff() {
		return emgOff;
	}

	public void setEmgOff(boolean emgOff) {
		this.emgOff = emgOff;
	}

	public boolean isDoWork() {
		return doWork;
	}

	public void setDoWork(boolean doWork) {
		this.doWork = doWork;
	}

	public int getDistance() {
		return distance;
	}

	public void setDistance(int distance) {
		this.distance = distance;
	}

	public MotorCmd getLeftM() {
		return leftM;
	}

	public void setLeftM(MotorCmd leftM) {
		this.leftM = leftM;
	}

	public MotorCmd getRightM() {
		return rightM;
	}

	public void setRightM(MotorCmd rightM) {
		this.rightM = rightM;
	}

}
