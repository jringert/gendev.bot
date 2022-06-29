package gendev.bot.runner.aspectj;

public privileged aspect FrontCheck {

	pointcut execFwd(ExecutorHandler eh) : 
		execution(void *.forward*(..)) && target(eh);
	
	void around (ExecutorHandler eh) : execFwd(eh) {
		if (eh.getDistance() < 50) {
			eh.turning();
		} else {
			proceed(eh);
		}
	}
}
