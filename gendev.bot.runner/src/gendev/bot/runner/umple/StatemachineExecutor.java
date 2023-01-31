package gendev.bot.runner.umple;

import gendev.bot.runner.Executor;

public class StatemachineExecutor extends Executor {
	private static AbstractBot bot = new BotImpl();;
	private boolean prevWork = false;
	private boolean prevEmgStop = false;

	@Override
	public void nextStep() {
		bot.setDistance(getDistance());
		if (bot.getDistance() < 20) {
			bot.obstacle();
		} else {
			bot.clear();
		}

		if (isEmgOff()) {
			bot.emgStop();
		}
		if (isDoWork()) {
			bot.doWork();
		}

		if (!isEmgOff() && prevEmgStop) {
			bot.releaseEmgStop();
		}
		prevEmgStop = isEmgOff();
		if (!isDoWork() && prevWork) {
			bot.doneWorking();
		}
		prevWork = isDoWork();

		setLeftM(bot.getLeftM());
		setRightM(bot.getRightM());
	}

}
