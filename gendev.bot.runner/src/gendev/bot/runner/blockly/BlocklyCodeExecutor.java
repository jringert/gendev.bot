package gendev.bot.runner.blockly;

import gendev.bot.runner.Executor;
import gendev.bot.runner.umple.AbstractBot.MotorCmd;

public class BlocklyCodeExecutor extends Executor {

	@Override
	public void nextStep() {
		// TODO paste generated code here
		if (getDistance() < 10) {
		    drive("right");
		} else {
		    drive("forward");
		}
		
		
//		if (isEmgOff()) {
//		    drive("stop");
//		} else {
//		    if (getDistance() < 25) {
//		      if (getDistance() < 10) {
//		        drive("backward");
//		    } else {
//		        drive("right");
//		    }
//		  } else {
//		      drive("forward");
//		  }
//		}
	}

	private void drive(String dir) {
		switch (dir) {
		case "stop":
			setLeftM(MotorCmd.STP);
			setRightM(MotorCmd.STP);
			break;
		case "forward":
			setLeftM(MotorCmd.FWD);
			setRightM(MotorCmd.FWD);
			break;
		case "backward":
			setLeftM(MotorCmd.BWD);
			setRightM(MotorCmd.BWD);
			break;
		case "left":
			setLeftM(MotorCmd.BWD);
			setRightM(MotorCmd.FWD);
			break;
		case "right":
			setLeftM(MotorCmd.FWD);
			setRightM(MotorCmd.BWD);
			break;			
		}
	}
}
