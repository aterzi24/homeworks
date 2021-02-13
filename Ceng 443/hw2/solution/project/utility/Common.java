package project.utility;

import project.components.Factory;
import project.parts.*;
import project.parts.logics.*;
import project.parts.payloads.*;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Common
{
    public static Random random = new Random() ;
    // names of all payloads and logics
    public static final List<String> payloadNames = new ArrayList<>();
    public static final List<String> logicNames = new ArrayList<>();

    static {
        // order is important for comparing
        payloadNames.add("Gripper");
        payloadNames.add("Welder");
        payloadNames.add("Camera");
        payloadNames.add("MaintenanceKit");

        logicNames.add("Supplier");
        logicNames.add("Builder");
        logicNames.add("Inspector");
        logicNames.add("Fixer");
    }

    public static synchronized Object get (Object object , String fieldName )
    {
        // TODO
        // This function retrieves (gets) the private field of an object by using reflection
        // In case the function needs to throw an exception, throw this: SmartFactoryException( "Failed: get!" )
        try
        {
            Field field = object.getClass().getDeclaredField(fieldName);
            field.setAccessible(true);
            return field.get(object);
        }
        catch (Exception e)
        {
            throw new SmartFactoryException("Failed: get!");
        }
    }

    public static synchronized void set ( Object object , String fieldName , Object value )
    {
        // TODO
        // This function modifies (sets) the private field of an object by using reflection
        // In case the function needs to throw an exception, throw this: SmartFactoryException( "Failed: set!" )
        try
        {
            Field field = object.getClass().getDeclaredField(fieldName);
            field.setAccessible(true);
            field.set(object, value);
        }
        catch (Exception e)
        {
            throw new SmartFactoryException("Failed: set!");
        }

    }

    // this function returns a base whose serialNo is no
    public static Base createBase(int no) {
        return new Base(no);
    }

    // this function returns a part according to its name
    public static Part createPart(String name) {
        switch (name) {
            case "Arm":             return new Arm();
            case "Builder":         return new Builder();
            case "Fixer":           return new Fixer();
            case "Inspector":       return new Inspector();
            case "Supplier":        return new Supplier();
            case "Camera":          return new Camera();
            case "Gripper":         return new Gripper();
            case "MaintenanceKit":  return new MaintenanceKit();
            case "Welder":          return new Welder();
            default:                return null;
        }
    }

    // this function returns a random part
    public static Part createRandomPart() {
        switch (random.nextInt(4))
        {
            case 0: return Factory.createBase();
            case 1: return new Arm();
            case 2: return createPart(payloadNames.get(random.nextInt(payloadNames.size())));
            default: return createPart(logicNames.get(random.nextInt(logicNames.size())));
        }
    }

    // this function assembles the part on base if appropriate
    // return true if part is assembled on base, false otherwise
    public static boolean assemble(Base base, Arm arm, Payload payload, Part part) {
        String name = part.getClass().getSimpleName();
        // determine what is part
        if (name.equals("Base"))
            return false;
        if (name.equals("Arm"))
        {
            if (arm == null)
            {
                set(base, "arm", part);
                return true;
            }
            else
                return false;
        }
        if (payloadNames.contains(name))
        {
            if ((arm != null) && (payload == null))
            {
                set(base, "payload", part);
                return true;
            }
            else
                return false;
        }
        if (logicNames.contains(name))
        {
            if (payload != null)
            {
                int index1 = payloadNames.indexOf(payload.getClass().getSimpleName());
                int index2 = logicNames.indexOf(name);
                if (index1 == index2)
                {
                    set(base, "logic", part);
                    return true;
                }
            }
        }
        return false;
    }
}