package com.defold.androidnativeext;

import android.os.Vibrator;
import android.content.Context;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;

class NativeExample {

    public static final void vibratePhone(Context context, int vibrateMilliSeconds) {
        Vibrator vibrator = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
        vibrator.vibrate(vibrateMilliSeconds);
    }
    
    public static String DoStuff() {
        return "Message From Java!";
    }
    
    public static String GetRaw(Context context) {
        try {
            InputStream inputStream = context.getResources().openRawResource(R.raw.test);
            InputStreamReader inputreader = new InputStreamReader(inputStream);
            BufferedReader buffreader = new BufferedReader(inputreader);
            String line;
            StringBuilder text = new StringBuilder();
            while (( line = buffreader.readLine()) != null) {
                text.append(line);
                text.append('\n');
            }
            return text.toString();
        } catch (Exception e) {
            return "io exception: " + e.getMessage();
        }
    } 
}
