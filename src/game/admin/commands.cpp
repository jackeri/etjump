#include <bitset>
#include <boost/algorithm/string.hpp>
#include "commands.hpp"
#include "../g_local.hpp"
#include "../g_save.hpp"
#include "session.hpp"

typedef boost::function<bool(gentity_t *ent, Arguments argv)> Command;
typedef std::pair<boost::function<bool(gentity_t *ent, Arguments argv)>, char> AdminCommandPair;
typedef std::map< std::string, boost::function<bool(gentity_t *ent, Arguments argv)> >::const_iterator ConstCommandIterator;
typedef std::map< std::string, std::pair<boost::function<bool(gentity_t *ent, Arguments argv)>, char> >::const_iterator ConstAdminCommandIterator;
typedef std::map< std::string, boost::function<bool(gentity_t *ent, Arguments argv)> >::iterator CommandIterator;
typedef std::map< std::string, std::pair<boost::function<bool(gentity_t *ent, Arguments argv)>, char> >::iterator AdminCommandIterator;

namespace CommandFlags
{
    const char BAN = 'b';
    // For everyone
    const char BASIC = 'a';
    const char CANCELVOTE = 'C';
    const char EBALL = '8';
    const char EDIT = 'A';
    const char FINGER = 'f';
    const char HELP = 'h';
    const char KICK = 'k';
    const char LISTBANS = 'L';
    const char LISTPLAYERS = 'l';
    const char MAP = 'M';
    const char MUTE = 'm';
    const char NOCLIP = 'N';
    const char NOGOTO = 'K';
    const char PASSVOTE = 'P';
    const char PUTTEAM = 'p';
    const char READCONFIG = 'G';
    const char RENAME = 'R';
    const char RESTART = 'r';
    const char SAVESYSTEM = 'T';
    const char SETLEVEL = 's';
}

namespace ClientCommands
{
    bool BackupLoad(gentity_t *ent, Arguments argv)
    {
        game.saves->LoadBackupPosition(ent);
        return true;
    }

    bool Load(gentity_t *ent, Arguments argv)
    {
        game.saves->Load(ent);
        return true;
    }

    bool Save(gentity_t *ent, Arguments argv)
    {
        game.saves->Save(ent);
        return true;
    }
}

void PrintManual(gentity_t *ent, const std::string& command)
{
    if (ent)
    {
        ChatPrintTo(ent, va("^3%s: ^7check console for more information.", command.c_str()));
        trap_SendServerCommand(ent->client->ps.clientNum, va("manual %s", command.c_str()));
    }
    else
    {
        for (int i = 0; i < sizeof(commandManuals) / sizeof(commandManuals[0]); i++)
        {
            if (!Q_stricmp(commandManuals[i].cmd, command.c_str()))
            {
                G_Printf("%s\n\nUsage:\n%s\n\nDescription:\n%s\n",
                    commandManuals[i].cmd, commandManuals[i].usage,
                    commandManuals[i].description);
                return;
            }
        }
    }
}

bool IsTargetHigherLevel(gentity_t *ent, gentity_t *target, bool equalIsHigher)
{
    if (equalIsHigher)
    {
        return game.session->GetLevel(ent) <= game.session->GetLevel(target);
    }

    return game.session->GetLevel(ent) < game.session->GetLevel(target);
}

bool IsTargetHigherLevel(gentity_t *ent, unsigned id, bool equalIsHigher)
{
    if (equalIsHigher)
    {
        return game.session->GetLevel(ent) <= game.session->GetLevelById(id);
    }

    return game.session->GetLevel(ent) < game.session->GetLevelById(id);
}

namespace AdminCommands
{
    bool Admintest(gentity_t *ent, Arguments argv)
    {
        if (!ent)
        {
            ChatPrintAll("^3admintest: ^7console is a level ? user.");
            return true;
        }

        game.session->PrintAdmintest(ent);
        return true;
    }

    bool AddLevel(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("AddLevel");
        return true;
    }

    bool Ball8(gentity_t* ent, Arguments argv)
    {
        static const std::string Magical8BallResponses[] = {
            "It is certain",
            "It is decidedly so",
            "Without a doubt",
            "Yes - definitely",
            "You may rely on it",

            "As I see it, yes",
            "Most likely",
            "Outlook good",
            "Signs point to yes",
            "Yes",

            "Reply hazy, try again",
            "Ask again later",
            "Better not tell you now",
            "Cannot predict now",
            "Concentrate and ask again",

            "Don't count on it",
            "My reply is no",
            "My sources say no",
            "Outlook not so good",
            "Very doubtful"
        };
        const int DELAY_8BALL = 3000; // in milliseconds

        if (ent && ent->client->last8BallTime + DELAY_8BALL > level.time)
        {
            ChatPrintTo(ent, va("^3!8ball: ^7you must wait %i seconds before using !8ball again.", (ent->client->last8BallTime + DELAY_8BALL - level.time) / 1000));
            return false;
        }

        if (argv->size() != 2)
        {
            PrintManual(ent, "8ball");
            return false;
        }

        int random = rand() % 20;
        const int POSITIVE = 10;
        const int NO_IDEA = 15;
        if (random < POSITIVE)
        {
            ChatPrintAll("^3Magical 8 Ball: ^2" + Magical8BallResponses[random]);
        }
        else if (random < NO_IDEA)
        {
            ChatPrintAll("^3Magical 8 Ball: ^3" + Magical8BallResponses[random]);
        }
        else
        {
            ChatPrintAll("^3Magical 8 Ball: ^1" + Magical8BallResponses[random]);
        }

        if (ent)
        {
            ent->client->last8BallTime = level.time;
        }
        return true;
    }

    bool Ban(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("Ban");
        return true;
    }

    bool Cancelvote(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("Cancelvote");
        return true;
    }

    bool DeleteLevel(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("DeleteLevel");
        return true;
    }

    bool DeleteUser(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("DeleteUser");
        return true;
    }

    bool EditCommands(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("EditCommands");
        return true;
    }

    bool EditLevel(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("EditLevel");
        return true;
    }

    bool EditUser(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("EditUser");
        return true;
    }    

    bool AdminCommands(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("AdminCommands");
        return true;
    }

    bool Finger(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("Finger");
        return true;
    }

    bool Help(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("Help");
        return true;
    }

    bool Kick(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("Kick");
        return true;
    }

    bool LevelInfo(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("LevelInfo");
        return true;
    }

    bool ListBans(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("ListBans");
        return true;
    }

    bool ListCommands(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("ListCommands");
        return true;
    }

    bool ListFlags(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("ListFlags");
        return true;
    }

    bool ListMaps(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("ListMaps");
        return true;
    }

    bool ListPlayers(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("ListPlayers");
        return true;
    }

    bool ListUsers(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("ListUsers");
        return true;
    }

    bool Map(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("Map");
        return true;
    }

    bool MapInfo(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("MapInfo");
        return true;
    }

    bool Mute(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("Mute");
        return true;
    }

    bool Noclip(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("Noclip");
        return true;
    }

    bool NoGoto(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("NoGoto");
        return true;
    }

    bool NoSave(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("NoSave");
        return true;
    }

    bool Passvote(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("Passvote");
        return true;
    }

    bool Putteam(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("Putteam");
        return true;
    }

    bool ReadConfig(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("ReadConfig");
        return true;
    }

    bool RemoveSaves(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("RemoveSaves");
        return true;
    }

    bool Rename(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("Rename");
        return true;
    }

    bool Restart(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("Restart");
        return true;
    }

    bool SetLevel(gentity_t* ent, Arguments argv)
    {
        // !setlevel <player> <level>
        // !setlevel -id id level

        if (argv->size() == 3)
        {
            std::string err;
            gentity_t *target = PlayerGentityFromString(argv->at(1), err);

            if (!target)
            {
                ChatPrintTo(ent, err);
            }

            int level = 0;
            if (!ToInt(argv->at(2), level))
            {
                ChatPrintTo(ent, "^3setlevel: ^7invalid level " + argv->at(2));
                return false;
            }

            if (ent)
            {
                if (IsTargetHigherLevel(ent, target, false))
                {
                    ChatPrintTo(ent, "^3setlevel: ^7you can't set the level of a fellow admin.");
                    return false;
                }

                if (level > game.session->GetLevel(ent))
                {
                    ChatPrintTo(ent, "^3setlevel: ^7you're not allowed to setlevel higher than your own level.");
                    return false;
                }
            }

            if (!game.levels->LevelExists(level))
            {
                ChatPrintTo(ent, "^3setlevel: ^7level does not exist.");
                return false;
            }

            if (!game.session->SetLevel(target, level))
            {
                ChatPrintTo(ent, va("^3setlevel: ^7%s", game.session->GetMessage().c_str()));
                return false;
            }

            ChatPrintTo(ent, va("^3setlevel: ^7%s^7 is now a level %d user.", target->client->pers.netname, level));
            ChatPrintTo(target, va("^3setlevel: ^7you are now a level %d user.", level));

            return true;
        }
        if (argv->size() == 4)
        {
            unsigned id = 0;
            if (!ToUnsigned(argv->at(2), id))
            {
                ChatPrintTo(ent, "^3setlevel: ^7invalid id " + argv->at(2));
                return false;
            }

            if (!game.session->UserExists(id))
            {
                ChatPrintTo(ent, "^3setlevel: ^7user with id " + argv->at(2) + " doesn't exist.");
                return false;
            }

            int level = 0;
            if (!ToInt(argv->at(3), level))
            {
                ChatPrintTo(ent, "^3setlevel: ^7invalid level " + argv->at(2));
                return false;
            }

            if (ent)
            {
                if (IsTargetHigherLevel(ent, id, false))
                {
                    ChatPrintTo(ent, "^3setlevel: ^7you can't set the level of a fellow admin.");
                    return false;
                }

                if (level > game.session->GetLevel(ent))
                {
                    ChatPrintTo(ent, "^3setlevel: ^7you're not allowed to setlevel higher than your own level.");
                    return false;
                }
            }

            if (!game.levels->LevelExists(level))
            {
                ChatPrintTo(ent, "^3setlevel: ^7level does not exist.");
                return false;
            }

            if (!game.session->SetLevel(id, level))
            {
                ChatPrintTo(ent, va("^3setlevel: ^7%s", game.session->GetMessage().c_str()));
                return false;
            }

            ChatPrintTo(ent, va("^3setlevel: ^7user with id %d is now a level %d user.", id, level));
        }
        else
        {
            
        }

        return true;
    }

    bool Spectate(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("Spectate");
        return true;
    }

    bool Unban(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("Unban");
        return true;
    }

    bool Unmute(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("Unmute");
        return true;
    }

    bool UserInfo(gentity_t* ent, Arguments argv)
    {
        G_LogPrintf("UserInfo");
        return true;
    }

    
    
}

Commands::Commands()
{
    //using AdminCommands::AdminCommand;
    //adminCommands_["addlevel"] = AdminCommand(AdminCommands::AddLevel, 'a');
    
    
    adminCommands_["addlevel"] = AdminCommandPair(AdminCommands::AddLevel, CommandFlags::EDIT);
    adminCommands_["admintest"] = AdminCommandPair(AdminCommands::Admintest, CommandFlags::BASIC);
    adminCommands_["8ball"] = AdminCommandPair(AdminCommands::Ball8, CommandFlags::BASIC);
    adminCommands_["ban"] = AdminCommandPair(AdminCommands::Ban, CommandFlags::BAN);
    adminCommands_["cancelvote"] = AdminCommandPair(AdminCommands::Cancelvote, CommandFlags::CANCELVOTE);
    adminCommands_["deletelevel"] = AdminCommandPair(AdminCommands::DeleteLevel, CommandFlags::EDIT);
    adminCommands_["deleteuser"] = AdminCommandPair(AdminCommands::DeleteUser, CommandFlags::EDIT);
    adminCommands_["editcommands"] = AdminCommandPair(AdminCommands::EditCommands, CommandFlags::EDIT);
    adminCommands_["editlevel"] = AdminCommandPair(AdminCommands::EditLevel, CommandFlags::EDIT);
    adminCommands_["edituser"] = AdminCommandPair(AdminCommands::EditUser, CommandFlags::EDIT);
    adminCommands_["finger"] = AdminCommandPair(AdminCommands::Finger, CommandFlags::FINGER);
    adminCommands_["help"] = AdminCommandPair(AdminCommands::Help, CommandFlags::BASIC);
    adminCommands_["kick"] = AdminCommandPair(AdminCommands::Kick, CommandFlags::KICK);
    adminCommands_["levelinfo"] = AdminCommandPair(AdminCommands::LevelInfo, CommandFlags::EDIT);
    adminCommands_["listbans"] = AdminCommandPair(AdminCommands::ListBans, CommandFlags::LISTBANS);
    adminCommands_["listcmds"] = AdminCommandPair(AdminCommands::ListCommands, CommandFlags::BASIC);
    adminCommands_["listflags"] = AdminCommandPair(AdminCommands::ListFlags, CommandFlags::EDIT);
    adminCommands_["listmaps"] = AdminCommandPair(AdminCommands::ListMaps, CommandFlags::BASIC);
    adminCommands_["listplayers"] = AdminCommandPair(AdminCommands::ListPlayers, CommandFlags::LISTPLAYERS);
    adminCommands_["listusers"] = AdminCommandPair(AdminCommands::ListUsers, CommandFlags::EDIT);
    adminCommands_["map"] = AdminCommandPair(AdminCommands::Map, CommandFlags::MAP);
    adminCommands_["mapinfo"] = AdminCommandPair(AdminCommands::MapInfo, CommandFlags::BASIC);
    adminCommands_["mute"] = AdminCommandPair(AdminCommands::Mute, CommandFlags::MUTE);
    adminCommands_["noclip"] = AdminCommandPair(AdminCommands::Noclip, CommandFlags::NOCLIP);
    adminCommands_["nogoto"] = AdminCommandPair(AdminCommands::NoGoto, CommandFlags::NOGOTO);
    adminCommands_["nosave"] = AdminCommandPair(AdminCommands::NoSave, CommandFlags::SAVESYSTEM);
    adminCommands_["passvote"] = AdminCommandPair(AdminCommands::Passvote, CommandFlags::PASSVOTE);
    adminCommands_["putteam"] = AdminCommandPair(AdminCommands::Putteam, CommandFlags::PUTTEAM);
    adminCommands_["readconfig"] = AdminCommandPair(AdminCommands::ReadConfig, CommandFlags::READCONFIG);
    adminCommands_["rmsaves"] = AdminCommandPair(AdminCommands::RemoveSaves, CommandFlags::SAVESYSTEM);
    adminCommands_["rename"] = AdminCommandPair(AdminCommands::Rename, CommandFlags::RENAME);
    adminCommands_["restart"] = AdminCommandPair(AdminCommands::Restart, CommandFlags::RESTART);
    adminCommands_["setlevel"] = AdminCommandPair(AdminCommands::SetLevel, CommandFlags::SETLEVEL);
    adminCommands_["spectate"] = AdminCommandPair(AdminCommands::Spectate, CommandFlags::BASIC);
    adminCommands_["unban"] = AdminCommandPair(AdminCommands::Unban, CommandFlags::BAN);
    adminCommands_["unmute"] = AdminCommandPair(AdminCommands::Unmute, CommandFlags::MUTE);
    adminCommands_["userinfo"] = AdminCommandPair(AdminCommands::UserInfo, CommandFlags::EDIT);

    commands_["backup"] = ClientCommands::BackupLoad;
    commands_["save"] = ClientCommands::Save;
    commands_["load"] = ClientCommands::Load;
}

bool Commands::ClientCommand(gentity_t* ent, std::string commandStr)
{
    G_DPrintf("Commands::ClientCommand called for %d\n", ClientNum(ent));

    ConstCommandIterator command = commands_.find(commandStr);
    if (command == commands_.end())
    {
        return false;
    } 
     
    command->second(ent, GetArgs());

    return true;
}

bool Commands::AdminCommand(gentity_t* ent)
{
    std::string command = "",
                arg = SayArgv(0);
    int skip = 0;

    if (arg == "say" || arg == "enc_say")
    {
        arg = SayArgv(1);
        skip = 1;
    }
    Arguments argv = GetSayArgs(skip);

    if (arg.length() == 0)
    {
        return false;
    }

    if (arg[0] == '!')
    {
        command = &arg[1];
    }
    else if (ent == NULL)
    {
        command = arg;
    }
    else
    {
        return false;
    }

    boost::to_lower(command);

    ConstAdminCommandIterator it = adminCommands_.lower_bound(command);

    if (it == adminCommands_.end())
    {
        return false;
    }
    std::bitset<256> permissions =
        game.session->Permissions(ent);
    std::vector<ConstAdminCommandIterator> foundCommands;
    while (it != adminCommands_.end() &&
        it->first.compare(0, command.length(), command) == 0) {
        if (permissions[it->second.second])
        {
            if (it->first == command)
            {
                foundCommands.clear();
                foundCommands.push_back(it);
                break;
            }
            foundCommands.push_back(it);
        }
        it++;
    }

    if (foundCommands.size() == 1)
    {
        foundCommands[0]->second.first(ent, argv);
    }
    else if (foundCommands.size() > 1)
    {
        ChatPrintTo(ent, "^3server: ^7multiple matching commands found. Check console for more information");
        BeginBufferPrint();
        for (size_t i = 0; i < foundCommands.size(); i++)
        {
            BufferPrint(ent, va("* %s\n", foundCommands.at(i)->first.c_str()));
        }
        FinishBufferPrint(ent);
    }

    return false;
}