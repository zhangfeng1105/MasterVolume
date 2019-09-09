package Util;

public class SoundAdminUtil {

	static {
		System.loadLibrary("SoundAdmin");
	}

	private static SoundAdminUtil soundAdminUtil = null;

	private SoundAdminUtil() throws OperationFailedException {
		init();
	}

	public static SoundAdminUtil getInstance() {
		if (soundAdminUtil == null) {
			try {
				soundAdminUtil = new SoundAdminUtil();
			} catch (OperationFailedException e) {
				e.printStackTrace();
				return null;
			}
		}
		return soundAdminUtil;
	}

	private native void init() throws OperationFailedException;

	public native void setMasterVolume(int num) throws OperationFailedException;

	public native int getMasterVolume() throws OperationFailedException;

	public native void setMute(boolean bMute) throws OperationFailedException;

	public native boolean getMute() throws OperationFailedException;

	private native void finished();

	@Override
	public void finalize() {
		finished();
	}
	public static void main(String[] args) {
		// 加载C文件

		SoundAdminUtil soundAdminUtil = SoundAdminUtil.getInstance();
		int sound;
		try {
			sound = soundAdminUtil.getMasterVolume();
			System.out.println(sound);
			soundAdminUtil.setMasterVolume(20);
			soundAdminUtil.setMute(!soundAdminUtil.getMute());
			soundAdminUtil.finalize();
		} catch (OperationFailedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}

