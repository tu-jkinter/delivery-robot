
namespace RobotBluetoothControl;

using System.Drawing;
using System.Text;
using SharpDX.XInput;

/// <summary>
/// Represents an attached <see cref="XboxController"/> to the user PC.
/// </summary>
public class XboxController
{
    //Constants
    private const int INPUT_DEADZONE = Gamepad.LeftThumbDeadZone;

    //Properties
    private Controller Controller { get; init; }
    private Gamepad Gamepad { get; set; }
    public int Deadband { get; set; } = 2500;
    public bool Connected { get; set; } = false;
    public ThumbstickDirection LeftThumb { get; set; } = ThumbstickDirection.Neutral;
    public ThumbstickDirection RightThumb { get; set; } = ThumbstickDirection.Neutral;

    public XboxController()
    {
        Controller = new Controller(UserIndex.One);
        Connected = Controller.IsConnected;
    }


    public override string ToString()
    {
        StringBuilder sb = new();
        sb.Append($"LeftThumb: {LeftThumb} | ");
        sb.Append($"RightThumb: {RightThumb}");
        return sb.ToString();
    }


    /// <summary>
    /// Updates all control values for this <see cref="XboxController"/>.
    /// </summary>
    public void Update()
    {
        if (!Connected) return;

        //Obtain the current gamepad state
        Gamepad = Controller.GetState().Gamepad;
        SetThumbStickDirections();
    }


    private void SetThumbStickDirections()
    {
        LeftThumb= GetThumbStickDirection(Gamepad.LeftThumbX, Gamepad.LeftThumbY);
        RightThumb = GetThumbStickDirection(Gamepad.RightThumbX, Gamepad.RightThumbY);
    }


    private static ThumbstickDirection GetThumbStickDirection(int x, int y)
    {
        double magnitude = Math.Sqrt(x*x + y*y);

        //If moving in a diagnonal direction
        if (magnitude <= INPUT_DEADZONE)
        {
            return ThumbstickDirection.Neutral;
        }

        double angle = GetAngle(x, y);

        return angle switch
        {
            _ when angle > 337.5 || angle <= 22.5 => ThumbstickDirection.Forward,
            _ when angle > 22.5 && angle <= 67.5 => ThumbstickDirection.ForwardRight,
            _ when angle > 67.5 && angle <= 112.5 => ThumbstickDirection.Right,
            _ when angle > 112.5 && angle <= 157.5 => ThumbstickDirection.BackwardRight,
            _ when angle > 157.5 && angle <= 202.5 => ThumbstickDirection.Backward,
            _ when angle > 202.5 && angle <= 247.5 => ThumbstickDirection.BackwardLeft,
            _ when angle > 247.5 && angle <= 292.5 => ThumbstickDirection.Left,
            _ when angle > 292.5 && angle <= 337.5 => ThumbstickDirection.ForwardLeft,
            _ => ThumbstickDirection.Neutral
        };
    }
     

    private static double GetAngle(int x, int y)
    {
        double radians = Math.Atan2((double) y, (double) x);
        double degrees = radians * (180 / Math.PI);
        degrees -= 90;

        if (degrees < 0)
        {
            return Math.Abs(degrees);
        }

        return Math.Abs(degrees - 360);
    }
}
