#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

// Defines message types, used in message handlers
enum MessageTypes
{
    Msg_DestroySource,
    Msg_TargetDestroyed,
    Msg_ShipSpeedChange,
    Msg_CtrlMod_SpecialKeyPress,
    Msg_SpecialKeyPress,
    Msg_SpecialKeyRelease,
    Msg_CtrlMod_SpecialKeyRelease,
    Msg_ShipWarp,
    Msg_ToggleGravity
};

#endif
