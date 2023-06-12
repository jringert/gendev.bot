package gendev.bot.runner.spectra;

import java.io.IOException;
import java.util.LinkedHashMap;
import java.util.Map;

import gendev.bot.runner.Executor;
import gendev.bot.runner.umple.AbstractBot.MotorCmd;
import tau.smlab.syntech.controller.executor.ControllerExecutor;
import tau.smlab.syntech.games.controller.StaticController;

public class SymbolicControllerExecutor extends Executor {
	private ControllerExecutor executor;
	private Map<String, String> envValues;
	private boolean iniState;

	public SymbolicControllerExecutor() {
		try {
			//executor = new ControllerExecutor(new BasicJitController(), "out/jit", "botV1");
			executor = new ControllerExecutor(new StaticController(), "out/static", "botV0");
		} catch (IOException e) {
			// unable to handle this case,
			throw new RuntimeException("unable to load symbolic JIT controller, was it synthesized?", e);
		}
		envValues = new LinkedHashMap<>();
		iniState = true;
	}

	@Override
	public void nextStep() {
		envValues.clear();
		setInputIfExists("emgStop", Boolean.toString(isEmgOff()));
		setInputIfExists("work", Boolean.toString(isDoWork()));
		setInputIfExists("distance", Integer.toString(getDistance()));

		if (iniState) {
			executor.initState(envValues);
			iniState = false;
		} else {
			executor.updateState(envValues);
		}

		Map<String, String> sysValues = executor.getCurrOutputs();

		setLeftM(MotorCmd.valueOf(sysValues.get("leftM")));
		setRightM(MotorCmd.valueOf(sysValues.get("rightM")));
	}

	public void setInputIfExists(String var, String val) {
		if (executor.getEnvVars().containsKey(var)) {
			try {
				envValues.put(var, val);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

}
