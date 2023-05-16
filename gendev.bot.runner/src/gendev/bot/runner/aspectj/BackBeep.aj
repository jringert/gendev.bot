package gendev.bot.runner.aspectj;

import gendev.bot.runner.RobotRunnerGui;

public privileged aspect BackBeep {

	pointcut backing() : 
		call(void *.backing(..)) && target(JavaAspectExecutor);
	
	before() : backing() {
		RobotRunnerGui.instance.beep();
	}
}
