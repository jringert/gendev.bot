package gendev.bot.runner.aspectj;

public privileged aspect BackBeep {

	pointcut backing(JavaAspectExecutor eh) : 
		call(void *.backing(..)) && target(eh);
	
	before(JavaAspectExecutor eh) : backing(eh) {
		eh.gui.beep();
	}
}
