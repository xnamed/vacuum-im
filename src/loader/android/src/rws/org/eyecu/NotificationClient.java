/*
    ************************************************************
*/

package rws.org.eyecu;

import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import org.qtproject.qt5.android.bindings.QtActivity;

import android.os.Bundle;
import android.view.Gravity;
import android.widget.Toast;

/*
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.BitmapFactory;
import android.support.v7.app.ActionBarActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
*/



/*
#define rws_org_eyecu_NotificationClient_NO         0x00000000
#define rws_org_eyecu_NotificationClient_SOUND      0x00000001
#define rws_org_eyecu_NotificationClient_VIBRATE    0x00000002
#define rws_org_eyecu_NotificationClient_LIGHTS     0x00000004
#define rws_org_eyecu_NotificationClient_ALL        0x00000007
*/


public class NotificationClient extends QtActivity
{
    private static NotificationManager  ANotificationManager;
    private static Notification.Builder ABuilder;
    private static NotificationClient   AInstance;

    public NotificationClient()
    {
        AInstance = this;
    }

    public static void notify(String AMessage,String ATitle,String AId,String ARegim)
    {
        if (ANotificationManager == null)
        {
            ANotificationManager = (NotificationManager)AInstance.getSystemService(Context.NOTIFICATION_SERVICE);
            ABuilder = new Notification.Builder(AInstance);
            ABuilder.setSmallIcon(R.drawable.icon);
            //---additional features
            ABuilder.setAutoCancel(true);
            ABuilder.setTicker("Message from eyeCU");
        }
        switch (Integer.parseInt(ARegim))
        {
            case 0: ABuilder.setDefaults(0);
                break;
            case 1: ABuilder.setDefaults(Notification.DEFAULT_SOUND);   //Constant Value: 1 (0x00000001)
                break;
            case 2: ABuilder.setDefaults(Notification.DEFAULT_VIBRATE); //Constant Value: 2 (0x00000002)
                break;
            case 4: ABuilder.setDefaults(Notification.DEFAULT_LIGHTS);  //Constant Value: 4 (0x00000004)
                break;
            case 3: ABuilder.setDefaults(Notification.DEFAULT_SOUND | Notification.DEFAULT_VIBRATE);
                break;
            case 5: ABuilder.setDefaults(Notification.DEFAULT_LIGHTS | Notification.DEFAULT_SOUND);
                break;
            case 6: ABuilder.setDefaults(Notification.DEFAULT_LIGHTS | Notification.DEFAULT_VIBRATE);
                break;
            case 7: ABuilder.setDefaults(Notification.DEFAULT_ALL);     //Constant Value: -1 (0xffffffff)
                break;
             default: ABuilder.setDefaults(Notification.DEFAULT_ALL);
        }
        ABuilder.setContentTitle(ATitle);
        ABuilder.setContentText(AMessage);
        ANotificationManager.notify(Integer.parseInt(AId),ABuilder.build());
    }

    public static void notifydelete(int AId)
    {
        if (ANotificationManager != null)
        {
            ANotificationManager = (NotificationManager)AInstance.getSystemService(Context.NOTIFICATION_SERVICE);
            ANotificationManager.cancel(AId);
        }
    }


    public static void toast(String AIcon,String AMessage,String ATitle,String AGravity,String ATime)
    {
        Toast toast = Toast.makeText(AInstance.getApplicationContext(),AMessage,Toast.LENGTH_LONG);
        toast.setGravity(Gravity.CENTER, 0, 0);
        toast.show();
/*
        int duration;
        switch (Integer.parseInt(ATime))
        {
            case 0: duration = Toast.LENGTH_SHORT;  break;
            case 1: duration = Toast.LENGTH_LONG;   break;
            default:duration = Toast.LENGTH_SHORT;
        }
        Toast toast = Toast.makeText(AInstance.getApplicationContext(),AMessage,duration);

        switch (Integer.parseInt(AGravity))
        {
            case 0: toast.setGravity(Gravity.LEFT  | Gravity.TOP, 0, 0); break;
            case 1: toast.setGravity(Gravity.TOP, 0, 0);                break;
            case 2: toast.setGravity(Gravity.RIGHT | Gravity.TOP, 0, 0);break;
            case 3: toast.setGravity(Gravity.LEFT  | Gravity.CENTER, 0, 0);break;
            case 4: toast.setGravity(Gravity.CENTER, 0, 0);             break;
            case 5: toast.setGravity(Gravity.RIGHT | Gravity.CENTER, 0, 0); break;
            case 6: toast.setGravity(Gravity.LEFT  | Gravity.BOTTOM, 0, 0); break;
            case 7: toast.setGravity(Gravity.BOTTOM, 0, 0);             break;
            case 8: toast.setGravity(Gravity.RIGHT | Gravity.BOTTOM, 0, 0); break;
            default: toast.setGravity(Gravity.CENTER, 0, 0); break;
        }
        toast.show();
*/
    }
}
