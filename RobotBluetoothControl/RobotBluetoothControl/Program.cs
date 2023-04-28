
namespace RobotBluetoothControl;

using System.IO.Ports;

public class Program
{
    public static void Main()
    {
        //Create an xbox controller link (must have controller plugged into PC)
        XboxController controller = new();

        //Create link with HC-05 bluetooth module COM port
        SerialPort port = BluetoothController.CreateAndOpenPort();

        //While port remains open, monitor remote activity and push to HC-05 bluetooth module.
        while (port.IsOpen)
        {
            controller.Update();
            string bluetoothString = BluetoothController.MapXboxControl(controller);

            if (port.TryWrite(bluetoothString))
            {
                continue;
            }

            Console.WriteLine($"Port {port.PortName} has disconnected. Attempting reconnect...");
            if (port.Reconnect())
            {
                Console.WriteLine($"Port {port.PortName} has successfully reconnected.");
                continue;
            }

            Console.WriteLine($"Port {port.PortName} has failed to reconnect. [Program ended]");

            try
            {
                port.Close();
                port.Dispose();
                break;
            }
            catch
            {
                break;
            }
        }
    }
}