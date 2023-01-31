package gendev.bot.runner.spectra;

import java.util.Map;

import gendev.bot.runner.Executor;
import gendev.bot.runner.umple.AbstractBot.MotorCmd;
import tau.smlab.syntech.executor.ControllerExecutor;
import tau.smlab.syntech.executor.ControllerExecutorException;

public class SymbolicControllerExecutor extends Executor {
	private ControllerExecutor executor = new ControllerExecutor(true, true);

	@Override
	public void nextStep() {
		setInputIfExists("emgStop", Boolean.toString(isEmgOff()));
		setInputIfExists("work", Boolean.toString(isDoWork()));
		setInputIfExists("distance", Integer.toString(getDistance()));

		try {
			executor.updateState(true);
		} catch (ControllerExecutorException e) {
			e.printStackTrace();
		}

		Map<String, String> sysValues = executor.getCurOutputs();

		setLeftM(MotorCmd.valueOf(sysValues.get("leftM")));
		setRightM(MotorCmd.valueOf(sysValues.get("rightM")));
	}
	
	public void setInputIfExists(String var, String val) {	
		if (executor.getEnvVarNames().contains(var)) {
			try {
				executor.setInputValue(var, val);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

}
