package gendev.bot.runner.aspectj;

public privileged aspect BackBeep {

	pointcut backing(ExecutorHandler eh) : 
		call(void *.backing(..)) && target(eh);
	
	before(ExecutorHandler eh) : backing(eh) {
		eh.gui.beep();
	}
}
