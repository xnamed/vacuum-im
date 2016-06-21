/*
    ************************************************************
*/

package rws.eyecu;

import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import org.qtproject.qt5.android.bindings.QtActivity;

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
            case 3: ABuilder.setDefaults(Notification.DEFAULT_LIGHTS);  //Constant Value: 4 (0x00000004)
                break;
            case 4: ABuilder.setDefaults(Notification.DEFAULT_SOUND | Notification.DEFAULT_VIBRATE);
                break;
            case 5: ABuilder.setDefaults(Notification.DEFAULT_LIGHTS | Notification.DEFAULT_VIBRATE);
                break;
            case 6: ABuilder.setDefaults(Notification.DEFAULT_LIGHTS | Notification.DEFAULT_SOUND);
                break;
            case 9: ABuilder.setDefaults(Notification.DEFAULT_ALL);     //Constant Value: -1 (0xffffffff)
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
}
