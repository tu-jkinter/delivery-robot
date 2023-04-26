
using System.IO.Ports;

namespace RobotBluetoothControl;

public class BluetoothController
{
    /// <summary>
    /// Obtain the Bluetooth client associated with the HC-05
    /// </summary>
    /// <returns>The <see cref="SerialPort"/>, which is not open.</returns>
    public static SerialPort CreatePort()
    {
        return new("COM13", 9600, Parity.None, 8, StopBits.One)
        {
            DtrEnable = true,
            RtsEnable = true,
            WriteTimeout = 200,
        };
    }


    /// <summary>
    /// Create and attempt to open the port post creation. 
    /// </summary>
    /// <returns>The <see cref="SerialPort"/>, which may be open.</returns>
    public static SerialPort CreateAndOpenPort()
    {
        SerialPort port = CreatePort();

        Console.WriteLine($"Attempting to connect to HC-05 through port {port.PortName}.");

        //Attempt to open bluetooth port
        try
        {
            port.Open();
            Console.WriteLine("Successfully connected to HC-05.");
            return port;
        }
        catch (Exception ex)
        {
            Console.WriteLine(ex);
            Console.WriteLine("Port to HC-05 could not be successfully opened.");
            return port;
        }
    }


    /// <summary>
    /// Map the output of the <see cref="XboxController"/> to a bluetooth character for robot control.
    /// </summary>
    /// <param name="controller"></param>
    /// <returns>The <see cref="string"/> that represents the bluetooth control <see cref="char"/>.</returns>
    public static string MapXboxControl(XboxController controller)
    {
        if (!controller.Connected)
        {
            return "M";
        }

        //Priority 1: Translational Movement
        if (controller.LeftThumb != ThumbstickDirection.Neutral)
        {
            return GetMovementDirection(controller.LeftThumb);
        }
        
        //Priority 2: Rotational Movement 
        if (controller.RightThumb != ThumbstickDirection.Neutral)
        {
            return GetRotationalDirection(controller.RightThumb);
        }

        return "M";
    }


    private static string GetMovementDirection(ThumbstickDirection thumbstickDirection)
    {
        return thumbstickDirection switch
        {
            ThumbstickDirection.Forward => "A",
            ThumbstickDirection.Backward => "B",
            ThumbstickDirection.Right => "C",
            ThumbstickDirection.Left => "D",
            ThumbstickDirection.ForwardRight => "E",
            ThumbstickDirection.ForwardLeft => "F",
            ThumbstickDirection.BackwardRight => "G",
            ThumbstickDirection.BackwardLeft => "H",
            _ => "M",
        };
    }


    private static string GetRotationalDirection(ThumbstickDirection thumbstickDirection)
    {
        return thumbstickDirection switch
        {
            ThumbstickDirection.Right => "I", //Rotate clockwise
            ThumbstickDirection.Left => "J", //Rotate counterclockwise
            ThumbstickDirection.Forward => "K", //Arc clockwise
            ThumbstickDirection.Backward => "L", //Arc counterclockwise
            _ => "M",
        };
    }
}
