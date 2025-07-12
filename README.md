# UE5-NetDormancy-Demo
This is a simple Unreal Engine 5 project that demonstrates how to use **Net Dormancy** to optimize network replication for actors in multiplayer games.

🧪 What This Project Shows

This project includes two test cases:

1. **Explosive Barrels (Always Awake)**
   - Barrels are set to replicate constantly using `NetDormancy = Awake`.
   - Even while idle, they continue sending data over the network.
   - When damaged, they explode and spawn cosmetic effects.

2. **Explosive Barrels (DormantAll)**
   - Barrels start with `NetDormancy = DormantAll`.
   - They send no updates over the network until they take damage.
   - On impact, they wake up from dormancy, replicate effects, and get destroyed.

These two setups are analyzed using **Unreal's Network Profiler**, showing the difference in replication cost and network waste.

---

## 📦 Push-Based Replication

This project also uses **Push-Based Replication**, which helps reduce bandwidth by only replicating variables when explicitly marked as "dirty".

### 🔧 How to Enable Push Replication

To enable push-based replication in your project, you must add `NetCore` as a dependency module in your `YourProject.Build.cs` file.

PublicDependencyModuleNames.AddRange(new string[] {
    "Core",
    "CoreUObject",
    "Engine",
    "InputCore",
    "EnhancedInput",
    "UMG",
    "NetCore" // necessary for push replication
});

---

Video link:
https://www.linkedin.com/posts/enespa%C3%A7arizi_networking-optimization-in-unreal-engine-activity-7349812386809180164-gSJk?utm_source=share&utm_medium=member_desktop&rcm=ACoAAD99Wu4BXueQKsS11JdnI6k-UKQrKb1jd24

---

NetProfiler Results

| Mode             | Processing Time | Replication Waste |
|------------------|------------------|--------------------|
| Always Awake     | ~15ms            | ~99.32%            |
| DormantAll       | ~0.13ms          | 0%                 |

---

✍️ Author

**Enes Pacarizi**  
