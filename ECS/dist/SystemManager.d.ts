import { System } from './System';
import { World } from './World';
export declare class SystemManager {
    private systems;
    private world;
    setWorld(world: World): void;
    addSystem(system: System): void;
    removeSystem(system: System): void;
    getSystem<T extends System>(systemClass: new (...args: any[]) => T): T | undefined;
    private sortSystems;
    update(deltaTime: number): void;
    clear(): void;
    getSystems(): System[];
}
//# sourceMappingURL=SystemManager.d.ts.map