import { System } from './System';
import { World } from './World';

export class SystemManager {
    private systems: System[] = [];
    private world!: World;

    public setWorld(world: World): void {
        this.world = world;
    }

    public addSystem(system: System): void {
        system.setWorld(this.world);
        this.systems.push(system);
        this.sortSystems();
    }

    public removeSystem(system: System): void {
        const index = this.systems.indexOf(system);
        if (index !== -1) {
            this.systems.splice(index, 1);
        }
    }

    public getSystem<T extends System>(systemClass: new (...args: any[]) => T): T | undefined {
        return this.systems.find(s => s instanceof systemClass) as T | undefined;
    }

    private sortSystems(): void {
        this.systems.sort((a, b) => b.priority - a.priority);
    }

    public update(deltaTime: number): void {
        for (const system of this.systems) {
            if (system.enabled) {
                system.preUpdate(deltaTime);
                system.update(deltaTime);
                system.postUpdate(deltaTime);
            }
        }
    }

    public clear(): void {
        this.systems = [];
    }

    public getSystems(): System[] {
        return [...this.systems];
    }
}