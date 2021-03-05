/*PLEASE DO NOT EDIT THIS CODE*/
/*This code was generated using the UMPLE 1.30.1.5099.60569f335 modeling language!*/

package gendev.bot.runner.umple;

// line 5 "../../../../botV0.ump"
public class BotImpl extends AbstractBot
{

  //------------------------
  // MEMBER VARIABLES
  //------------------------

  //BotImpl State Machines
  public enum State {  }
  private State state;

  //------------------------
  // CONSTRUCTOR
  //------------------------

  public BotImpl()
  {
    super();
  }

  //------------------------
  // INTERFACE
  //------------------------

  public String getStateFullName()
  {
    String answer = state.toString();
    return answer;
  }

  public State getState()
  {
    return state;
  }

  public void delete()
  {}

}