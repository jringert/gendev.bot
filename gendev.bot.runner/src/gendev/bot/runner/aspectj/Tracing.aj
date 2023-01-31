package gendev.bot.runner.aspectj;

public privileged aspect Tracing {

	pointcut callPrivateMethods() : 
		call(private * JavaAspectExecutor.*(..));
	
	before() : callPrivateMethods() {
		System.out.println(thisJoinPoint.getSignature().getName());
	}
}
