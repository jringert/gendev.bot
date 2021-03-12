package gendev.bot.runner;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.io.File;

import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.swing.AbstractButton;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class RobotRunnerGui extends JPanel implements ActionListener {
	private static final long serialVersionUID = -3134321758170908129L;
	private boolean work = false;
	private boolean emgStop = false;

	private JButton b1;
	private JButton b2;
	private Clip clip;

	public RobotRunnerGui() {
		b1 = new JButton("Do Work");
		b1.setVerticalTextPosition(AbstractButton.CENTER);
		b1.setHorizontalTextPosition(AbstractButton.LEADING);
		b1.setMnemonic(KeyEvent.VK_D);
		b1.setActionCommand("work");
		b1.addActionListener(this);
		add(b1);

		b2 = new JButton("Emergency Stop");
		b2.setVerticalTextPosition(AbstractButton.CENTER);
		b2.setHorizontalTextPosition(AbstractButton.LEADING);
		b2.setActionCommand("emgStop");
		b2.addActionListener(this);
		add(b2);
	}

	public static RobotRunnerGui createAndShowGUI() {
		RobotRunnerGui gui = new RobotRunnerGui();
		javax.swing.SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				// Create and set up the window.
				JFrame frame = new JFrame("RobotRunnerGui");
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

				// Create and set up the content pane.
				gui.setOpaque(true); // content panes must be opaque
				frame.setContentPane(gui);

				// Display the window.
				frame.pack();
				frame.setVisible(true);
			}
		});
		return gui;
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if ("work".equals(e.getActionCommand())) {
			work = !work;
		}
		if ("emgStop".equals(e.getActionCommand())) {
			emgStop = !emgStop;
		}
		System.out.println(this.toString());
	}

	@Override
	public String toString() {
		return "RobotRunnerGui [work=" + work + ", emgStop=" + emgStop + "]";
	}

	public boolean isWork() {
		return work;
	}

	public boolean isEmgStop() {
		return emgStop;
	}

	public void beep() {
		if (clip == null || !clip.isActive()) {
			try {
				clip = AudioSystem.getClip();
				AudioInputStream ais = AudioSystem.getAudioInputStream(new File("beep.wav"));
				clip.open(ais);
				clip.start();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

}
