
import java.io.File;
import java.io.FileWriter;
import java.io.InputStream;
import java.io.OutputStream;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;
import java.util.Enumeration;

public class Serial implements SerialPortEventListener {

	SerialPort serialPort;
	File file = new File("C:/finedust0614.txt");
	FileWriter fw=null;

	private static final String PORT_NAMES[] = {

			"/dev/tty.usbserial-A9007UX1", // Mac OS X

			"/dev/ttyUSB0", // Linux

			"COM5", // Windows

	};
	private InputStream input;

	private OutputStream output;

	private static final int TIME_OUT = 2000;

	private static final int DATA_RATE = 9600;

	public void initialize() {

		CommPortIdentifier portId = null;

		Enumeration portEnum = CommPortIdentifier.getPortIdentifiers();

		while (portEnum.hasMoreElements()) {

			CommPortIdentifier currPortId = (CommPortIdentifier) portEnum.nextElement();

			for (String portName : PORT_NAMES) {

				if (currPortId.getName().equals(portName)) {

					portId = currPortId;

					break;

				}

			}

		}

		if (portId == null) {

			System.out.println("Could not find COM port.");

			return;

		}

		try {
			serialPort = (SerialPort) portId.open(this.getClass().getName(),

					TIME_OUT);

			serialPort.setSerialPortParams(DATA_RATE,

					SerialPort.DATABITS_8,

					SerialPort.STOPBITS_1,

					SerialPort.PARITY_NONE);

			input = serialPort.getInputStream();

			output = serialPort.getOutputStream();

			serialPort.addEventListener(this);

			serialPort.notifyOnDataAvailable(true);

		} catch (Exception e) {

			System.err.println(e.toString());

		}

	}

	public synchronized void close() {

		if (serialPort != null) {

			serialPort.removeEventListener();

			serialPort.close();

		}

	}

	public synchronized void serialEvent(SerialPortEvent oEvent) {

		if (oEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) {

			try {
				
				fw=new FileWriter(file,true);
				

				int available = input.available();

				byte chunk[] = new byte[available];

				input.read(chunk, 0, available);

				System.out.print(new String(chunk)); 
				fw.write(new String(chunk));
				fw.close();

			} catch (Exception e) {

				System.err.println(e.toString());

			}

		}
	}

	public static void main(String[] args) throws Exception {

		Serial main = new Serial();

		main.initialize();

		System.out.println("Started");

	}

}