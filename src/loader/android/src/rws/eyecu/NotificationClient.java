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
    private static NotificationManager ANotificationManager;
    private static Notification.Builder ABuilder;
    private static NotificationClient AInstance;

    public NotificationClient()
    {
        AInstance = this;
    }

    public static void notify(String AMessage,String ATitle,String AId,String ASound)
    {
        if (ANotificationManager == null)
        {
            ANotificationManager = (NotificationManager)AInstance.getSystemService(Context.NOTIFICATION_SERVICE);
            ABuilder = new Notification.Builder(AInstance);
            ABuilder.setSmallIcon(R.drawable.icon);
            ABuilder.setContentTitle(ATitle);
            switch (Integer.parseInt(ASound)){
                case 1: ABuilder.setDefaults(Notification.DEFAULT_SOUND);
                    break;
                case 2: ABuilder.setDefaults(Notification.DEFAULT_VIBRATE);
                    break;
                case 3: ABuilder.setDefaults(Notification.DEFAULT_SOUND | Notification.DEFAULT_VIBRATE);
                    break;
            }
            //---additional features
            ABuilder.setAutoCancel(true);
            ABuilder.setTicker("Message from eyeCU!");
        }
        ABuilder.setContentText(AMessage);
        ANotificationManager.notify(Integer.parseInt(AId),ABuilder.build());
    }

    public static void notifydelete(int AId)
    {
        ANotificationManager = (NotificationManager)AInstance.getSystemService(Context.NOTIFICATION_SERVICE);
        ANotificationManager.cancel(AId);
    }
}
