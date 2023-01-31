package gendev.bot.runner.aspectj;

public privileged aspect FrontCheck {

	pointcut execFwd(JavaAspectExecutor eh) : 
		execution(void *.forward*(..)) && target(eh);
	
	void around (JavaAspectExecutor eh) : execFwd(eh) {
		if (eh.getDistance() < 50) {
			eh.turning();
		} else {
			proceed(eh);
		}
	}
}
