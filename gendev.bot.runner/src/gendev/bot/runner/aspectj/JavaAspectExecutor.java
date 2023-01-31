package gendev.bot.runner.aspectj;

import gendev.bot.runner.Executor;
import gendev.bot.runner.umple.AbstractBot.MotorCmd;

public class JavaAspectExecutor extends Executor {

	public void nextStep() {

		if (!isEmgOff()) {
			if (getDistance() < 20) {
				backing();
			} else if (Math.random() > .5) {
				forwarding();
			} else {
				turning();
			}
		} else {
			stopping();
		}
	}

	private void stopping() {
		setLeftM(MotorCmd.STP);
		setRightM(MotorCmd.STP);		
	}

	private void turning() {
		setLeftM(MotorCmd.BWD);
		setRightM(MotorCmd.FWD);		
	}

	private void forwarding() {
		setLeftM(MotorCmd.FWD);
		setRightM(MotorCmd.FWD);		
	}

	private void backing() {
		setLeftM(MotorCmd.BWD);
		setRightM(MotorCmd.BWD);		
	}

}
