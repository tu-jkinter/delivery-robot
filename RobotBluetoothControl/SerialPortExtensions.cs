
namespace RobotBluetoothControl;

using System.IO.Ports;
using Polly;
using Polly.Retry;

public static class SerialPortExtensions
{
    private static RetryPolicy Retry { get; set; } = Policy.Handle<Exception>().WaitAndRetry(10, times => TimeSpan.FromMilliseconds(100));


    /// <summary>
    /// Attempt to write a <see cref="string"/> value to the given <see cref="SerialPort"/>.
    /// </summary>
    /// <param name="port"></param>
    /// <param name="message"></param>
    /// <returns><see langword="true"/> if the <see cref="SerialPort"/> is successfully written to; <see langword="false"/> otherwise.</returns>
    public static bool TryWrite(this SerialPort port, string message)
    {
        try
        {
            port.Write(message);
            return true;
        }
        catch
        {
            return false;
        }
    }


    /// <summary>
    /// Attempt to reconnect this <see cref="SerialPort"/> to its destination.
    /// </summary>
    /// <param name="port"></param>
    /// <returns><see langword="true"/> if the <see cref="SerialPort"/> successfully reconnects; <see langword="false"/> otherwise.</returns>
    public static bool Reconnect(this SerialPort port)
    {
        try
        {
            return Retry.Execute(() =>
            {
                port.Close();
                port.Open();
                return true;
            });
        }
        catch
        {
            return false;
        }
    }
}
